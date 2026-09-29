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
- [ ] **`Sieve(0)` and `Sieve(1)` write out of bounds** (`m_sieve[0]`, `m_sieve[1]`).
  ASan doesn't catch this because `vector<bool>` isn't instrumented.
- [ ] **`leastCommonMultiple(0, 0)` raises SIGFPE (confirmed).** It divides by
  `gcd(0, 0) == 0`. It also overflows silently. Consider `std::gcd` and `std::lcm`
  instead. `lcm_gcd.hpp` uses `std::swap` without including `<utility>`.
- [ ] **`AmicableNumbers::d` overflows (confirmed).** `d(4294967040)` wraps to
  2265733248, because the sum of proper divisors can exceed 2³². Return a wider type.
- [ ] **`sievePrimes` misbehaves at its limits.**
  - It uses `unsigned int` loop counters against an `unsigned long` limit, so limits
    ≥ 2³² truncate or loop forever.
  - It returns `{2}` for limits ≤ 2.
  - It duplicates `Sieve`. Remove it, or implement it with `Sieve`.

## 2. Design and API

- [ ] **`Factor` keeps mutable static state public.** Any caller can change `primes`,
  `cache` and `cacheLock`. Make them private implementation details.
- [ ] **`Factor`'s cache costs more than it saves.**
  - It grows without bound: every composite that hits the cache stores its own entry.
  - `factor()` locks the mutex once per trial prime, which can be up to 6 542 times per
    call.
  - Measure against plain trial division bounded by √n. It is likely faster to drop
    the cache.
- [ ] **`Factor::primes` is built when the library loads.** Every program that loads the
  library pays for it, and any use during static initialization in another translation
  unit hits the static-initialization-order problem. Use a function-local static.
- [ ] **There are three different "unsigned" checks:** `utility::is_unsigned` in
  `digits.hpp`, the global `Unsigned` concept in `pow.hpp`, and `std::is_unsigned` in
  `isqrt.hpp`. `Unsigned` is `!numeric_limits<T>::is_signed`, which any type without a
  `numeric_limits` specialization satisfies. Replace all three with one concept in
  `utility`.
- [ ] **Names leak into the global namespace:** the `ModuloOverflow` and `Unsigned`
  concepts, and `Factor`'s `operator<`, `operator==` and `operator<<`. Move them into
  `utility`, and replace the hand-written comparisons with a defaulted `operator<=>`.
- [ ] **`utility::Number` is a namespace-wide alias** defined in `amicable_numbers.hpp`.
  Such a generic name clashes in spirit with the template parameters called `Number` in
  `lcm_gcd.hpp` and `digits.hpp`.
- [ ] **`Factor` and `AmicableNumbers` only handle 32 bits,** while the rest
  of the library is templated. Decide whether they should be templated too.
- [ ] **`millerRabin` bounds its bases with floating-point `log` and Bach's bound, which
  assumes the Riemann hypothesis.** The fixed base set is already proven for 64 bits.
  Use it directly, or use a smaller proven set (e.g. Sinclair's 7 bases).
- [ ] **`toNumber` doesn't detect overflow** and mixes signed and unsigned arithmetic
  when `ResultType` is signed.
- [ ] **`__attribute__((visibility("default")))` does nothing on templates and inline
  functions.** Use a single `NUMERIC_UTILITY_API` macro, e.g. from CMake's
  `GenerateExportHeader`, on the exported non-template symbols.
- [x] **Clean up `pow.hpp`.** A comment contains stray text
  (`target_link_libraries(test_socket …)`), and the header includes `<numeric>` when
  it needs `<limits>`.
- [ ] **Headers are installed flat into `include/`** (e.g. `~/include/pow.hpp`), and
  their guards are generic (`POW_HPP`, `ISQRT_HPP`), so they can collide with other
  libraries. Install them under `include/numeric_utility/` and prefix the guards.

## 3. Build and packaging

- [ ] **`CMAKE_CXX_REQUIRED` is a typo** for `CMAKE_CXX_STANDARD_REQUIRED`, so C++20 is
  not actually required.
- [ ] **The benchmark always builds** and needs `oscpp` at configure time, so the
  project won't configure without it. `benchmark/CMakeLists.txt` also passes `CONFIG`
  as a Boost component. Put the benchmark behind an option, or skip it when `oscpp` is
  missing.
- [ ] **No warning flags are set.** `-Wall -Wextra -Wconversion` points straight at the
  Miller-Rabin truncation in `pow.cpp` and `miller_rabin.cpp`. Enable warnings and fix
  what they report.
- [ ] **The package config doesn't call `find_dependency(Boost)`,** which
  `digits_multiprecision.hpp` needs.
- [ ] **Tests are added even when `BUILD_TESTING` is off,** and `include(CTest)` comes
  after `enable_testing()`. Guard the test directory with `BUILD_TESTING`.
- [ ] **Installed programs rely on `DYLD_LIBRARY_PATH` / `LD_LIBRARY_PATH`.** Set
  `CMAKE_INSTALL_RPATH` instead.
- [ ] **There is no CI, and `.gitignore` ignores `.github`,** so workflow files can't be
  committed.
- [ ] **The README is out of date.** It mentions operating-system utilities that aren't
  here, and it has no list of modules or usage examples.

## 4. Tests

- [ ] **Nothing tests `sievePrimes`.**
- [x] **`millerRabin` is tested with one value (65537).** The benchmark's comparison
  with the sieve stops at 10⁶, below where the bugs start. Add known 64-bit primes and
  strong pseudoprimes (e.g. 3215031751, 3825123056546413051), and a cross-check against
  the sieve.
- [x] **Test `isqrt` at every k² − 1 / k² boundary up to the type's maximum,** for each
  unsigned width.
- [ ] **Test `pow` and `powmod` near their limits,** comparing against an
  `unsigned __int128` reference.
- [ ] **Test `Sieve` with large inputs and at its edges:** 0, 1, 2, `size²`, and
  repeated calls to `isPrime`. Done except sieve sizes 0, 1 and 2, which wait on the
  out-of-bounds fix.
