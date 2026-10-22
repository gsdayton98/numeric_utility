# TODO

The items from the 2026-09-28 review are all done; see the git history for what was fixed and why.

## Documentation

- [x] **Fill out the API documentation.** The Doxygen site builds (`-DNUMERIC_UTILITY_DOCS=ON`, target
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

## Matrix and Dijkstra

`Matrix` and `Dijkstra` (`matrix.hpp`, `dijkstra.hpp`) came from projecteuler's euler83 in version 2.0.

- [ ] **Say "no cell" and "unreached" directly.** `Undefined` and `Infinity` are sentinels: a move off the grid
  returns `{Undefined, Infinity}`, and an unreachable target returns `Undefined` as if it were a distance.
  `std::optional`, or a list of in-range neighbours, would say it, and `operator()` could return an optional or throw.
- [ ] **Template the value type** (and maybe the distance type). Everything is `unsigned int`. A path sum that
  overflows is dropped now (the target then reads `Undefined`), so it is safe but limited.

## Ideas

- [ ] **A shared decimal-digit number** (raised 2026-10-13 in projecteuler, porting euler55 and 56 to Rust).
      The Rust ports keep each number as a `Vec<u8>` of decimal digits, least significant first, and do the
      arithmetic digit by digit: euler55 adds a number to its reverse, euler56 multiplies by a small factor in
      place, and euler57 adds. There are no big-integer conversions and no `rug`. Timings: euler55 0.02s in C++
      (`cpp_int`) against 0.01s in Rust, and euler56 0.02s against 0.005s, so this is about tidiness, not speed.
      - **Why it could be worth it in C++:** euler55 calls `utility::toDigits(cpp_int)` and `toNumber` twice per
        iteration, which is repeated big division, and then adds in `cpp_int`; a digit vector needs only the
        add-with-carry. euler55, 56 and 57 would also drop `Boost::headers` (Boost.Multiprecision) from their
        projecteuler `CMakeLists.txt`. It is one small type for three problems, next to `digits.hpp`, so it
        isn't copied three times, and the Python and Rust ports mirror it.
      - **Operations it needs:** construct from an integer, `add`, `add(reversed)`, `multiplyBySmall`, `size()`,
        `digitSum()`, `isPalindrome()`. Nothing more.
      - **Where it does not help:** the Russian peasant method (halve one factor, double the other, add when odd).
        For euler56 each step multiplies by `a < 100`, which is one O(n) pass with a carry; doubling and adding would
        be about 7 doublings and up to 7 additions, 14 passes. It only wins when addition is all there is. The same
        holds for squaring a power: euler56 needs every a^b for b < 100, so "previous power times a" already
        shares the work. euler55 only adds.
      - **What would save passes:** a bigger base, as `cpp_int` does inside: limbs of 10⁹ in `u32` (or 10¹⁸ in
        `u64`) do 9 to 18 times fewer passes, with a cheap per-limb digit split for the sum. Only worth it if
        something were slow, and nothing here is.
      - **Decided 2026-10-14:** the Rust ports keep their hand-rolled digit vectors (see Decisions in projecteuler's
        `rust/TODO.md`). Which approach the C++ takes, `cpp_int` as now or a numeric_utility digit type, is still open.
      - Optional. `cpp_int` is correct and fast enough, and projecteuler's CLAUDE.md prefers numeric_utility over local copies, so
        this is the way to do it if it's done at all.
