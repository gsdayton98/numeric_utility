# TODO

The items from the 2026-09-28 review are all done; see the git history for what was fixed and why.

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
