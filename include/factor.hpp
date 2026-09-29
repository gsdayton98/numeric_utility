// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2022 Glen S. Dayton. Rights reserved according to terms of included license.
#ifndef FACTOR_HPP
#define FACTOR_HPP
#include <compare>
#include <iosfwd>
#include <vector>
namespace utility {
    struct __attribute__((visibility("default")))  Factor {
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
    [[maybe_unused]] auto __attribute__((visibility("default"))) operator<<(std::ostream&, const Factor&) -> std::ostream&;
}

#endif //FACTOR_HPP
