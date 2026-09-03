// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2022 Glen S. Dayton. Rights reserved according to terms of included license.
#ifndef FACTOR_HPP
#define FACTOR_HPP
#include <map>
#include <mutex>
#include <vector>
namespace utility {
    struct __attribute__((visibility("default")))  Factor {
        static std::vector<unsigned int> primes;

        static std::mutex cacheLock;
        static std::map<unsigned int, std::vector<Factor> > cache;

        unsigned int prime; /// Prime factor of a number.
        unsigned int exponent;  /// Number of times the prime factor occurs within the number.

        /**
         * Preload a cache with pre-computed factors of numbers.
         * @param upperLimit Upper limit of numbers to preload their factors.
         */
        static auto preloadCache(unsigned int upperLimit) -> void;

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
    };
}

// Help testing and debugging
[[maybe_unused]] auto __attribute__((visibility("default"))) operator<(const utility::Factor& left, const utility::Factor& right) -> bool;
[[maybe_unused]] auto __attribute__((visibility("default"))) operator==(const utility::Factor& left, const utility::Factor& right) -> bool;
[[maybe_unused]] auto __attribute__((visibility("default"))) operator<<(std::ostream&, const utility::Factor&) -> std::ostream&;

#endif //FACTOR_HPP
