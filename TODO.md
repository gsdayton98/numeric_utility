# TODO

The items from the 2026-09-28 review are all done; see the git history for what was fixed and why.

## Performance

- [ ] **Speed up `millerRabin` for small `n`.** It always uses Sinclair's seven 64-bit bases, so every prime
  costs seven full `powmod` rounds, each `uint64_t` multiply going through a 128-bit product and `%` (a
  `__umodti3` call). projecteuler's euler60 makes about 1.1 million calls on 10-digit numbers at roughly
  830 ns each (measured 2026-10-01, Release), which is nearly all of its 1.7s.
  - Choose the bases by size: {2, 7, 61} is deterministic below 4,759,123,141, which covers every `uint32_t`
    and euler60's concatenations (below 3×10^9). Keep the seven bases above that.
  - Below 2^32, `x * x` fits in 64 bits, so square and multiply with plain `uint64_t` arithmetic instead of
    the 128-bit path.
  - The squaring loop calls `powmod(x, 2, n)`; a direct `mulmod(x, x, n)` skips the exponent loop.
  - Trial division by a few small primes (3, 5, 7, ...) first rejects most composites cheaply.
  - Measure with `benchmark/benchmark_isprime.cpp` before and after. Add 4,759,123,141 (the smallest strong
    pseudoprime to bases 2, 7 and 61) to `test_miller_rabin_large_composites`, and primes just below and above
    the cutover, so a wrong threshold fails a test.

## Documentation

- [ ] **Fill out the API documentation.** The Doxygen site builds (`-DNUMERIC_UTILITY_DOCS=ON`, target
  `docs`), but many classes and functions have partial or no doc comments.
  - Turn on `DOXYGEN_WARN_IF_UNDOCUMENTED` in `docs/CMakeLists.txt` to list what is missing, and work
    through it header by header.
  - Give every public function `@brief`, `@param`, `@return`, and `@throws` where it throws
    (`std::overflow_error`, `std::range_error`), plus its preconditions and complexity where they matter.
  - Give every class and concept a description, and every header a `@file` block.
  - Convert the `////` banner comments (e.g. `pow.hpp`, `pow_multiprecision.hpp`) to `/** @brief ... */`;
    Doxygen currently uses the banner's title line, such as "Pow(base, exponent).", as the brief.
  - Add `@code` examples for the main entry points, reusing the README examples.
  - Once the warnings are clean, set `DOXYGEN_WARN_AS_ERROR` so the Docs workflow keeps them clean.
