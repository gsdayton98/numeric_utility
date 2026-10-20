// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2022 Glen S. Dayton. Rights reserved according to terms of included license.
#ifndef NUMERIC_UTILITY_FACTOR_HPP
#define NUMERIC_UTILITY_FACTOR_HPP
#include <compare>
#include <iosfwd>
#include <vector>
#include <numeric_utility/numeric_utility_export.h>
/// @file
/// @brief Prime factorization of 32-bit numbers.
namespace utility {
    /// A prime and the number of times it divides a number.
    ///
    /// Deliberately 32-bit, unlike the templated rest of the library. factor() trial divides
    /// by the primes below 2^16, which covers any 32-bit number cheaply. Covering 64 bits
    /// would take a different algorithm, such as Pollard's rho with millerRabin, rather than
    /// a template parameter: trial division would need every prime below 2^32.
    ///
    /// @code
    /// for (const utility::Factor& f : utility::Factor::factor(360u)) std::cout << f << ' ';   // 2^3 3^2 5^1
    /// @endcode
    struct NUMERIC_UTILITY_API Factor {
        unsigned int prime;     ///< Prime factor of a number.
        unsigned int exponent;  ///< Number of times the prime factor occurs within the number.

        /**
         * Factor a number into primes.
         *
         * Trial divides by the primes below 2^16, so the cost is at worst about 6,500 divisions.
         * @param n Number to factor.
         * @return The prime factors of n in increasing order of prime. Empty for 0 and 1, which have none.
         */
        static auto factor(unsigned int n) -> std::vector<Factor>;

        /**
         * Evaluate a vector of factors, the inverse of factor().
         * @param factors The prime factors to multiply out.
         * @return The product of prime^exponent over the factors, 1 if there are none. Wraps modulo 2^32 if the
         *         product doesn't fit, as it won't for factors that came from factor().
         */
        static auto evaluate(const std::vector<Factor>& factors) -> unsigned int;

        /// Order by prime, then by exponent.
        /// @return The three-way comparison of the two factors.
        auto operator<=>(const Factor&) const = default;
    };

    /// Write the factor as prime^exponent, such as 2^3.
    /// @param out The stream to write to.
    /// @param factor The factor to write.
    /// @return out.
    [[maybe_unused]] NUMERIC_UTILITY_API auto operator<<(std::ostream& out, const Factor& factor) -> std::ostream&;
}

#endif //NUMERIC_UTILITY_FACTOR_HPP
