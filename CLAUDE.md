# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Working environment

- **Never commit, stage or push anything except `CLAUDE.md`, and `TODO.md`.  ** Every
  other file is the owner's to review: leave code changes in the working tree and suggest a commit message (with the
  `Co-Authored-By:` trailer) instead. Commit those four files yourself, and only those, on the current non-default branch.
- Do not commit a code change together with a TODO change: commit the TODO file by itself (`git commit <file>`).

- Open work is tracked in `TODO.md`

## Build directories

Keep build trees out of the source tree. Claude's builds live in `../build/numeric_utility/claude/`:

- `build`: Debug build.
- `build-asan`: Debug build with AddressSanitizer and UndefinedBehaviorSanitizer.

`../build/numeric_utility/cmake-build-debug` and `cmake-build-release` belong to the IDE; leave them alone.

CMake build trees contain absolute paths, so don't move them: configure a new tree and delete the old one.

```bash
B=../build/numeric_utility/claude
cmake -S . -B $B/build -DCMAKE_BUILD_TYPE=Debug
cmake -S . -B $B/build-asan -DCMAKE_BUILD_TYPE=Debug "-DNUMERIC_UTILITY_SANITIZERS=address;undefined"
cmake --build $B/build -j && $B/build/test/test_utility
cmake --build $B/build-asan -j && UBSAN_OPTIONS=halt_on_error=1 $B/build-asan/test/test_utility
```
