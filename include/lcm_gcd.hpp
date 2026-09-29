// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2022 Glen S. Dayton. Rights reserved according to terms of included license.
#ifndef LCM_GCD_HPP
#define LCM_GCD_HPP
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>


namespace utility {
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
     * Least common multiple of numbersIn; 1 if it is empty.
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
#endif //LCM_GCD_HPP
