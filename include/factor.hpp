// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2022 Glen S. Dayton. Rights reserved according to terms of included license.
#ifndef FACTOR_HPP
#define FACTOR_HPP
#include <compare>
#include <iosfwd>
#include <vector>
#include "numeric_utility_export.h"
namespace utility {
    /// A prime and the number of times it divides a number.
    ///
    /// Deliberately 32-bit, unlike the templated rest of the library. factor() trial divides
    /// by the primes below 2^16, which covers any 32-bit number cheaply. Covering 64 bits
    /// would take a different algorithm, such as Pollard's rho with millerRabin, rather than
    /// a template parameter: trial division would need every prime below 2^32.
    struct NUMERIC_UTILITY_API Factor {
        unsigned int prime; /// Prime factor of a number.
        unsigned int exponent;  /// Number of times the prime factor occurs within the number.

        /**
         * Attempt to factor a number.
         *
         * @param n Number to factor.
         * @return a vector of factors of the number n.
         */
        static auto factor(unsigned int n) -> std::vector<Factor>;

        /**
         * Evaluate a vector of factors.
         * @return The number the factors form.
         */
        static auto evaluate(const std::vector<Factor>&) -> unsigned int;

        /// Order by prime, then by exponent.
        auto operator<=>(const Factor&) const = default;
    };

    /// Write the factor as prime^exponent.
    [[maybe_unused]] NUMERIC_UTILITY_API auto operator<<(std::ostream&, const Factor&) -> std::ostream&;
}

#endif //FACTOR_HPP
