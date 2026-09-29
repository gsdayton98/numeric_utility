# TODO

Findings from a review of the library on 2026-09-28. Items marked **(confirmed)** were
reproduced against the built library. The others come from reading the code.

## 1. Correctness bugs

- [x] **`millerRabin` was wrong for n > 2³².** It truncated each squaring to 32 bits
  (`auto y = 0u`), on top of the `powmod` overflow, and reported primes such as
  999 999 999 989 and 2⁶⁴−59 as composite. Fixed, with tests of 64-bit primes and
  strong pseudoprimes, and a comparison with the sieve below 10⁶.
- [x] **`powmod` overflowed when the modulus's square didn't fit in the type.**
  Fixed with specializations for `uint8_t`, `uint16_t`, `uint32_t`, `uint64_t` and
  `unsigned __int128`, defined in `src/pow.cpp` and declared in `pow.hpp`. `cpp_int`
  has an overload in `pow_multiprecision.hpp`. `millerRabin` now takes `uint64_t`.
- [x] **The generic `powmod` still overflows for 64-bit types that aren't `uint64_t`.**
  Specializations match exact types, and `uint64_t` is `unsigned long long` on macOS
  but `unsigned long` on Linux. The other one, including `size_t` on macOS, gets the
  generic template, as do types such as Boost's fixed-width `uint128_t`. Accepted as is:
  callers use the fixed-width types.
- [x] **`isqrt` hung or gave wrong answers for large inputs.** `isqrt(4294901760u)` hung,
  and `isqrt(131768u)` returned 65537 instead of 362. Replaced with Newton's method
  from an overflow-safe starting guess, tested exhaustively for 8 and 16 bits, at every
  32-bit perfect square, and at 64-bit squares and type maximums.
- [x] **`Factors` was incomplete.** `operator/` implied rational numbers, which it did
  not implement: `asNumber` never returned for a negative exponent, and `operator<<` did
  not link. Moved to `project_euler_extras`; no Project Euler solution used it.
- [x] **`Sieve::isPrime` gave wrong answers and corrupted the prime list.**
  `Sieve(11).isPrime(121)` returned true, and beyond the sieve a `const` method appended
  to `m_primes`: `last()` changed, a repeated `isPrime(9973)` returned false because 9973
  then divided itself, and pointers into `primes()` (as euler60 keeps) could dangle. It
  now trial divides up to √n without modifying the sieve, throws `range_error` for
  n ≥ size², and `primes()` returns a `const` reference.
- [x] **`Sieve(0)` and `Sieve(1)` wrote out of bounds** (`m_sieve[0]`, `m_sieve[1]`).
  `Sieve(0)` crashed through a null pointer; `Sieve(1)`'s write stayed inside the
  allocated word, where only libc++ hardening catches it. Fixed by raising every limit
  to at least `MINIMUM_SIEVE_SIZE` (16). The sanitizer build now enables standard
  library hardening, which catches `vector<bool>` errors that ASan misses.
- [x] **`leastCommonMultiple(0, 0)` raised SIGFPE.** It divided by `gcd(0, 0) == 0`, as
  did the vector overload for any list with two zeros. It now returns 0, like `std::lcm`.
  `lcm_gcd.hpp` also now includes `<utility>` for `std::swap`.
- [x] **`leastCommonMultiple` overflowed silently** when the result didn't fit in the type.
  It now throws `std::overflow_error` for built-in integer types, including
  `unsigned __int128`.
- [x] **`greatestCommonDivisor` was wrong for `cpp_int`.** `gcd(12, 18)` returned 12, and so
  `leastCommonMultiple` of 1 through 50 returned 50. `auto r = a % b;` captured a
  Boost.Multiprecision expression template that still referred to `a` and `b`, so
  assigning `a = b` changed `r`. `r` is now declared `Number`.
- [x] **`AmicableNumbers::d` overflowed.** `d(4294967040)` wrapped to 2265733248, because
  the sum of proper divisors can exceed 2³². It now returns `std::uint64_t`.
- [x] **`sievePrimes` misbehaved at its limits.** It used `unsigned int` loop counters
  against an `unsigned long` limit, returned `{2}` for limits ≤ 2, and duplicated `Sieve`.
  It is now implemented with `Sieve<unsigned long>`, trimmed to the primes below the limit.
- [x] **`Factor::factor(2147483648u)` (2³¹) crashed with SIGFPE (confirmed).**
  `primePower *= prime` in `src/factor.cpp` wrapped to 0, and the next `n % primePower`
  divided by zero. It now divides `n` by `prime` repeatedly, without building `primePower`.
  `testPrimePowerOverflow` covers 2³¹, 3²⁰ and 65521², and `testLimits` covers 2³²−1 and
  the largest 32-bit prime.

## 2. Design and API

- [x] **`Factor` kept mutable static state public.** Any caller could change `primes`,
  `cache` and `cacheLock`. They now live in an anonymous namespace in `src/factor.cpp`.
- [x] **`Factor`'s cache cost more than it saved.** It grew without bound and took its lock
  once per trial prime. Measured against plain trial division up to √n: euler47's range
  took 134 ms against 43 ms, 200 000 random 32-bit numbers took 8.7 s against 0.34 s, and
  peak memory was 21 MB against 1.5 MB. Removed, along with `preloadCache`.
- [x] **`Factor::primes` was built when the library loads.** Every program that loads the
  library paid for it, and any use during static initialization in another translation
  unit hit the static-initialization-order problem. The prime table and cache are now
  function-local statics.
- [x] **There were three different "unsigned" checks:** `utility::is_unsigned` in
  `digits.hpp`, the global `Unsigned` concept in `pow.hpp`, and `std::is_unsigned` in
  `isqrt.hpp`. `Unsigned` was `!numeric_limits<T>::is_signed`, which any type without a
  `numeric_limits` specialization satisfied. All three are now `utility::Unsigned` in
  `concepts.hpp`, which requires a specialized, integer, unsigned `numeric_limits` and
  excludes `bool`. `isqrt` and `toDigits` now also accept Boost's fixed-width unsigned types.
- [x] **Names leaked into the global namespace:** the `ModuloOverflow` concept, and
  `Factor`'s `operator<`, `operator==` and `operator<<`. `ModuloOverflow` is now in
  `utility`, in `concepts.hpp`; `Factor` has a defaulted `operator<=>`, which also gives
  `==`; and `operator<<` is in `utility`, found by argument-dependent lookup.
- [x] **`utility::Number` was a namespace-wide alias** defined in `amicable_numbers.hpp`.
  Such a generic name clashed in spirit with the template parameters called `Number` in
  `lcm_gcd.hpp` and `digits.hpp`. Removed; `AmicableNumbers` now says `unsigned int`,
  as `Factor` does.
- [x] **`Factor` and `AmicableNumbers` only handle 32 bits,** while the rest
  of the library is templated. Decided to keep them 32-bit: every caller stays below
  150 000, and 64 bits needs a different algorithm (e.g. Pollard's rho), not a template
  parameter, because trial division would need every prime below 2³². Documented in
  `factor.hpp` and `amicable_numbers.hpp`.
- [x] **`millerRabin` bounded its bases with floating-point `log` and Bach's bound, which
  assumes the Riemann hypothesis.** It now uses Sinclair's 7 bases, proven for all n < 2⁶⁴,
  with no bound: 7 modular exponentiations instead of up to 12.
- [x] **`toNumber` didn't detect overflow** and mixed signed and unsigned arithmetic
  when `ResultType` was signed. For built-in integer types it now does all the arithmetic
  in `ResultType` and throws `std::overflow_error` if the result, the base or a digit
  doesn't fit. `cpp_int` is unchanged.
- [x] **`__attribute__((visibility("default")))` did nothing on templates and inline
  functions.** Removed from `isqrt`, `greatestCommonDivisor` and `leastCommonMultiple`,
  which consumers instantiate themselves. The exported non-template symbols, the
  explicit instantiations and the `powmod` specializations now use `NUMERIC_UTILITY_API`,
  from the `numeric_utility_export.h` that CMake's `GenerateExportHeader` writes and
  installs with the other headers.
- [x] **Clean up `pow.hpp`.** A comment contains stray text
  (`target_link_libraries(test_socket …)`), and the header includes `<numeric>` when
  it needs `<limits>`.
- [x] **Headers were installed flat into `include/`** (e.g. `~/include/pow.hpp`), and
  their guards were generic (`POW_HPP`, `ISQRT_HPP`), so they could collide with other
  libraries. They now live in `include/numeric_utility/`, are installed there, and are
  included as `<numeric_utility/pow.hpp>`. The guards are `NUMERIC_UTILITY_<NAME>_HPP`.

## 3. Build and packaging

- [x] **`CMAKE_CXX_REQUIRED` is a typo** for `CMAKE_CXX_STANDARD_REQUIRED`, so C++20 is
  not actually required.
- [x] **The benchmark always builds** and needs `oscpp` at configure time, so the
  project won't configure without it. `benchmark/CMakeLists.txt` also passes `CONFIG`
  as a Boost component. Put the benchmark behind an option, or skip it when `oscpp` is
  missing.
- [x] **No warning flags were set.** The library and the tests now build with
  `-Wall -Wextra -Wconversion`, and `-DNUMERIC_UTILITY_WERROR=ON` makes them errors. The
  Miller-Rabin truncation it was meant to catch was already fixed. It reported a signed loop
  counter in `amicable_numbers.cpp` and two conversions in `sieveprimes.hpp`, all fixed. Clang
  gets `--system-header-prefix=boost/` to keep Boost's own warnings out of the output.
- [x] **Headers included `"numeric_utility_export.h"` without the directory,** which broke
  a fresh configure after the headers moved; only a stale generated copy in old build trees
  hid it. They now include `<numeric_utility/numeric_utility_export.h>`.
- [x] **The package config didn't call `find_dependency(Boost)`,** which
  `digits_multiprecision.hpp` and `pow_multiprecision.hpp` need. The config now calls it,
  and the library links `Boost::headers` publicly so consumers get Boost's include path.
  Checked with a consumer project built against an installed copy.
- [x] **Tests were added even when `BUILD_TESTING` was off,** and `include(CTest)` came
  after `enable_testing()`. `include(CTest)` now replaces `enable_testing()` at the top, and
  the test directory, with its Boost.Test dependency, is added only under `BUILD_TESTING`.
- [x] **Installed programs relied on `DYLD_LIBRARY_PATH` / `LD_LIBRARY_PATH`.** Nothing in
  this project installs a program; the affected ones link the installed library, whose install name
  was `@rpath/...`. On macOS the installed copy now has an absolute install name (`INSTALL_NAME_DIR`),
  checked with a consumer built and installed against it. An rpath link option on the exported target
  was tried and dropped: CMake strips it when the consumer installs, and `ld` warns about the duplicate.
  Linux has no equivalent, so the README says what a consumer must set. **Not tested on Linux.**
- [ ] **There is no CI, and `.gitignore` ignores `.github`,** so workflow files can't be
  committed.
- [ ] **The README is out of date.** It mentions operating-system utilities that aren't
  here, and it has no list of modules or usage examples.

## 4. Tests

- [x] **Nothing tests `sievePrimes`.**
- [x] **`millerRabin` is tested with one value (65537).** The benchmark's comparison
  with the sieve stops at 10⁶, below where the bugs start. Add known 64-bit primes and
  strong pseudoprimes (e.g. 3215031751, 3825123056546413051), and a cross-check against
  the sieve.
- [x] **Test `isqrt` at every k² − 1 / k² boundary up to the type's maximum,** for each
  unsigned width.
- [ ] **Test `pow` and `powmod` near their limits,** comparing against an
  `unsigned __int128` reference.
- [x] **Test `Sieve` with large inputs and at its edges:** 0, 1, 2, `size²`, and
  repeated calls to `isPrime`.
