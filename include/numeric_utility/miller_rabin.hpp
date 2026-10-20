// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2025 Glen S. Dayton. Rights reserved according to terms of included license.
#ifndef NUMERIC_UTILITY_MILLER_RABIN_HPP
#define NUMERIC_UTILITY_MILLER_RABIN_HPP
#include <cstdint>
#include <numeric_utility/numeric_utility_export.h>
/// @file
/// @brief Deterministic primality test for 64-bit numbers.
namespace utility {
    /**
     * Test whether a 64-bit number is prime, by the Miller-Rabin test.
     *
     * The answer is exact, not probabilistic: for every n below 2^64 a fixed set of bases (2, 7 and 61 below
     * 4,759,123,141, and seven others above) has no strong pseudoprime, so no randomness or unproven
     * hypothesis is involved. Small primes are trial divided first. The cost is a few modular exponentiations,
     * about O(log n) multiplications each.
     *
     * @code
     * utility::millerRabin(18'446'744'073'709'551'557ULL);   // true: 2^64 - 59 is the largest 64-bit prime
     * @endcode
     * @param n The number to test.
     * @return true if n is prime; false for 0, 1 and composites.
     */
    NUMERIC_UTILITY_API auto millerRabin(std::uint64_t n) -> bool;
}
#endif //NUMERIC_UTILITY_MILLER_RABIN_HPP
