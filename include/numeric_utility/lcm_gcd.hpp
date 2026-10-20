// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2022 Glen S. Dayton. Rights reserved according to terms of included license.
#ifndef NUMERIC_UTILITY_LCM_GCD_HPP
#define NUMERIC_UTILITY_LCM_GCD_HPP
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>
/// @file
/// @brief Greatest common divisor and least common multiple.


namespace utility {
    /**
     * Greatest common divisor of a and b, by Euclid's algorithm.
     *
     * Works for any type with %, comparison and assignment, including Boost.Multiprecision's integers.
     * Unlike std::gcd it does not mix or reject types, and it needs non-negative arguments.
     *
     * @code
     * utility::greatestCommonDivisor(12u, 18u);   // 6
     * @endcode
     * @tparam Number An integer type.
     * @param a A non-negative number.
     * @param b A non-negative number.
     * @return The largest number dividing both; the other argument if one is 0, and 0 for gcd(0, 0).
     */
    template<typename Number>
    auto greatestCommonDivisor(Number a, Number b) -> Number {
        if (b > a) {
            std::swap(a, b);
        }
        while (b > 0) {
            // Not auto: for Boost.Multiprecision that would hold an unevaluated expression of a and b,
            // which the assignments below would change.
            Number r = a % b;
            a = b;
            b = r;
        }
        return a;
    }


    /**
     * Least common multiple of a and b.
     *
     * @code
     * utility::leastCommonMultiple(4u, 6u);   // 12
     * @endcode
     * @tparam Number An integer type.
     * @param a A non-negative number.
     * @param b A non-negative number.
     * @return The smallest positive number both divide, or 0 if either argument is 0.
     * @throw overflow_error if the result does not fit in Number.
     */
    template<typename Number>
    [[maybe_unused]] auto leastCommonMultiple(Number a, Number b) -> Number {
        // 0 is the only common multiple of 0, and gcd(0, 0) == 0 cannot be divided by.
        if (a == 0 || b == 0) return 0;
        const Number quotient = a / greatestCommonDivisor(a, b);
        if constexpr (std::is_integral_v<Number>) {
            Number product;
            if (__builtin_mul_overflow(quotient, b, &product)) {
                throw std::overflow_error("leastCommonMultiple: result does not fit in the type");
            }
            return product;
        } else {
            // Arbitrary-precision types such as cpp_int cannot overflow.
            return quotient * b;
        }
    }


    /**
     * Least common multiple of a list of numbers.
     *
     * @code
     * utility::leastCommonMultiple(std::vector{2u, 3u, 4u, 5u});   // 60
     * @endcode
     * @tparam Number An integer type.
     * @param numbersIn The numbers, each non-negative.
     * @return The smallest positive number all of them divide; 1 if the list is empty, and 0 if it holds a 0.
     * @throw overflow_error if the result does not fit in Number.
     */
    template<typename Number>
    auto leastCommonMultiple(const std::vector <Number> &numbersIn) -> Number {
        Number lcm = 1UL;

        for (auto n: numbersIn) {
            lcm = leastCommonMultiple(lcm, n);
        }
        return lcm;
    }
}
#endif //NUMERIC_UTILITY_LCM_GCD_HPP
