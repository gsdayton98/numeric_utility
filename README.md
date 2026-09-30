# Utility Library

Number-theory and digit utilities in C++20: primes, factoring, integer roots, modular arithmetic and
digit conversion.

I originally developed these routines for Project Euler solutions, and they proved to be generally useful
outside of Project Euler, or least usable across many Project Euler problems.

**API documentation: <https://gsdayton98.github.io/numeric_utility/>**

Everything is in namespace `utility`. Include the headers as `<numeric_utility/name.hpp>`.

## Modules

| Header | What it provides |
| --- | --- |
| `sieveprimes.hpp` | `Sieve<T>`, a sieve of Eratosthenes: the primes below a limit, and `isPrime` for numbers up to the square of the limit. `sievePrimes` fills a vector with the primes below a limit. |
| `miller_rabin.hpp` | `millerRabin(n)`, a deterministic primality test, exact for every 64-bit `n`. |
| `factor.hpp` | `Factor::factor(n)` returns the prime factorization of a 32-bit `n`; `Factor::evaluate` multiplies it back. |
| `amicable_numbers.hpp` | `AmicableNumbers::d(n)`, the sum of the proper divisors of a 32-bit `n`, and `divisors`. |
| `isqrt.hpp` | `isqrt(c)`, the integer square root of any unsigned type, without overflow. |
| `lcm_gcd.hpp` | `greatestCommonDivisor` and `leastCommonMultiple`, for two numbers or a vector. The LCM throws `std::overflow_error` for built-in types when the result does not fit. |
| `pow.hpp` | `pow`, wrapping modulo the size of the type, and `powmod`, which is correct for any modulus for `uint8_t` to `uint64_t` and `unsigned __int128`. Other unsigned types get a generic version that overflows unless the modulus squared fits. |
| `digits.hpp` | `toDigits(n, base)`, least significant digit first, and `toNumber<T>(digits, base)`, which throws `std::overflow_error` if the result does not fit. |
| `concepts.hpp` | The `Unsigned` and `ModuloOverflow` concepts the other headers use. |
| `pow_multiprecision.hpp`, `digits_multiprecision.hpp` | `powmod` and `toDigits` for Boost's `cpp_int`. They are separate so only their users depend on Boost.Multiprecision. |

`Factor` and `AmicableNumbers` are deliberately 32-bit. Factoring larger numbers needs a different
algorithm, not a wider type.

## Examples

```cpp
#include <cstdint>
#include <iostream>
#include <numeric_utility/digits.hpp>
#include <numeric_utility/factor.hpp>
#include <numeric_utility/isqrt.hpp>
#include <numeric_utility/lcm_gcd.hpp>
#include <numeric_utility/miller_rabin.hpp>
#include <numeric_utility/pow.hpp>
#include <numeric_utility/sieveprimes.hpp>

int main()
{
    using namespace utility;

    for (const Factor& f : Factor::factor(360u)) std::cout << f << ' ';
    std::cout << '\n';                                                    // 2^3 3^2 5^1

    std::cout << isqrt(1'000'000'007u) << '\n';                           // 31622
    std::cout << greatestCommonDivisor(12u, 18u) << ' '
              << leastCommonMultiple(4u, 6u) << '\n';                     // 6 12
    std::cout << powmod<std::uint64_t>(2, 100, 1'000'000'007) << '\n';    // 976371285
    std::cout << millerRabin(18'446'744'073'709'551'557ULL) << '\n';      // 1: 2^64 - 59 is prime

    const Sieve<unsigned int> sieve(100);
    std::cout << sieve.size() << ' ' << sieve.isPrime(97) << '\n';        // 25 1

    const auto digits = toDigits(9075u);                                  // least significant first
    std::cout << static_cast<int>(digits[0]) << ' ' << toNumber<unsigned int>(digits) << '\n';  // 5 9075
}
```

The GCD, LCM and `powmod` functions also work on Boost's `cpp_int`, which cannot overflow:

```cpp
#include <iostream>
#include <boost/multiprecision/cpp_int.hpp>
#include <numeric_utility/lcm_gcd.hpp>
#include <numeric_utility/pow_multiprecision.hpp>

int main()
{
    using boost::multiprecision::cpp_int;

    cpp_int lcm = 1;
    for (int i = 1; i <= 50; ++i) lcm = utility::leastCommonMultiple(lcm, cpp_int{i});
    std::cout << lcm << '\n';                                             // 3099044504245996706400

    std::cout << utility::powmod(cpp_int{3}, cpp_int{1000}, cpp_int{1'000'000'007}) << '\n';  // 56888193
}
```

## Building and installing

You need a C++20 compiler, CMake 3.25 or later, and Boost. Boost.Test is needed only to build the tests.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
ctest --test-dir build --output-on-failure
cmake --install build
```

The library is a shared library that installs to your home directory by default; set
`CMAKE_INSTALL_PREFIX` to change that. The CMake options are:

  * `BUILD_TESTING`: build the tests. On by default.
  * `NUMERIC_UTILITY_WERROR`: treat warnings as errors. Off by default.
  * `NUMERIC_UTILITY_SANITIZERS`: build the library and tests with sanitizers, for example
    `-DNUMERIC_UTILITY_SANITIZERS="address;undefined"`.
  * `NUMERIC_UTILITY_DOCS`: add a `docs` target that builds the API documentation. Off by default.

## Documentation

The [published documentation](https://gsdayton98.github.io/numeric_utility/) is rebuilt from `main` by CI.
To build it yourself: the headers' doc comments (Doxygen syntax, with LaTeX math) build into a searchable web site. You need
[Doxygen](https://www.doxygen.nl/), and optionally Graphviz for include graphs; CMake downloads the
[doxygen-awesome-css](https://github.com/jothepro/doxygen-awesome-css) theme when you configure.

```bash
cmake -S . -B build -DBUILD_TESTING=OFF -DNUMERIC_UTILITY_DOCS=ON
cmake --build build --target docs
open build/docs/html/index.html
```

## Using the library

From another CMake project:

```cmake
find_package(numeric_utility REQUIRED)
target_link_libraries(myprogram PRIVATE utility::numeric_utility)
```

If the library is installed outside CMake's search path, add its prefix to `CMAKE_PREFIX_PATH`.
Linking the target also brings in Boost's headers, which the two `*_multiprecision.hpp` headers need.

  * On macOS, the installed library records its absolute path, so programs that link it run without
any environment variables.

  * On Linux, a program that links the installed library needs an rpath to it, or `LD_LIBRARY_PATH` set to
the directory where the library is installed. With CMake, set `CMAKE_INSTALL_RPATH_USE_LINK_PATH` to `ON`
before defining the program's targets, or set `CMAKE_INSTALL_RPATH` to the library directory.

At this time I do not support installation or use on Windows.
