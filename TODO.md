# TODO

Findings from a review of the library on 2026-09-28. Items marked **(confirmed)** were
reproduced against the built library. The others come from reading the code.

## 1. Correctness bugs

- [ ] **`millerRabin` is still wrong for some n > 2³² (confirmed).** It reports the
  prime 2⁶⁴−59 as composite. The `powmod` overflow is fixed, and 4 294 967 311 and
  2⁶¹−1 are now reported correctly. What remains:
  - `auto y = 0u` in `millerRabin` truncates each `powmod` result to 32 bits.
  - `baseLimit` is an `unsigned int`.
  - The comment "n < 4,759,123,141" is the bound for the base set {2, 7, 61}. The base
    set {2 … 37} used here is deterministic for every 64-bit n.

  Fix those, and test against 64-bit primes and strong pseudoprimes.
- [x] **`powmod` overflowed when the modulus's square didn't fit in the type.**
  Fixed with an overflow-safe `mulmod` in `pow.hpp`. The broken specializations in
  `src/pow.cpp` were removed along with the file.
- [ ] **`isqrt` never returns for large inputs (confirmed).** `isqrt(4294901760u)` hangs.
  `x * x`, `(x + 1) * (x + 1)` and `2 * x` all overflow, starting from `x = c / 2`. Use
  an overflow-safe test (`x <= c / x`) or a bit-by-bit square root. The tests only
  cover 0–33.
- [ ] **`Factors::asNumber` never returns when an exponent is negative (confirmed).**
  `ipow` compares its `int` exponent with `0U`, so −1 compares as a huge unsigned value,
  and `b >>= 1` leaves it at −1. It also has to decide what a denominator means:
  divide, or throw when the result is not an integer.
- [ ] **`operator<<(std::ostream&, const Factors&)` doesn't link (confirmed).** It is
  declared in `utility` but defined in the global namespace in `src/factors.cpp`, so any
  use gets "Undefined symbols".
- [ ] **`Sieve::isPrime` gives wrong answers and corrupts the prime list (confirmed).**
  - `Sieve<unsigned>(11).isPrime(121)` returns true. The range check allows
    `number == size²`, but the primes found only go up to `size − 1`.
  - Beyond the sieve, a `const` method appends to `m_primes`. The list is left
    unsorted and gets duplicates: calling `isPrime(73)` twice adds 73 twice. It is also
    not thread-safe.
  - The loop stops at `2*divisor > number` rather than `divisor*divisor > number`, so it
    runs O(n) divisions instead of O(√n).
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
- [ ] **`utility::Number` is defined twice,** in `factors.hpp` and in
  `amicable_numbers.hpp`. The name also clashes in spirit with the template parameters
  called `Number` in `lcm_gcd.hpp` and `digits.hpp`.
- [ ] **`Factor`, `Factors` and `AmicableNumbers` only handle 32 bits,** while the rest
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

- [ ] **Nothing tests `Factors` or `sievePrimes`.**
- [ ] **`millerRabin` is tested with one value (65537).** The benchmark's comparison
  with the sieve stops at 10⁶, below where the bugs start. Add known 64-bit primes and
  strong pseudoprimes (e.g. 3215031751, 3825123056546413051), and a cross-check against
  the sieve.
- [ ] **Test `isqrt` at every k² − 1 / k² boundary up to the type's maximum,** for each
  unsigned width.
- [ ] **Test `pow` and `powmod` near their limits,** comparing against an
  `unsigned __int128` reference.
- [ ] **Test `Sieve` with large inputs and at its edges:** 0, 1, 2, `size²`, and
  repeated calls to `isPrime`.
