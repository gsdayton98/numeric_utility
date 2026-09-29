# CLAUDE.md

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
