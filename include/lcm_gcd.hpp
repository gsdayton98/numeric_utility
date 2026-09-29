// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2022 Glen S. Dayton. Rights reserved according to terms of included license.
#ifndef LCM_GCD_HPP
#define LCM_GCD_HPP
#include <utility>
#include <vector>


namespace utility {
    template<typename Number>
    auto __attribute__((visibility("default"))) greatestCommonDivisor(Number a, Number b) -> Number {
        if (b > a) {
            std::swap(a, b);
        }
        while (b > 0) {
            auto r = a % b;
            a = b;
            b = r;
        }
        return a;
    }


    template<typename Number>
    [[maybe_unused]] auto __attribute__((visibility("default"))) leastCommonMultiple(Number a, Number b) -> Number {
        // 0 is the only common multiple of 0, and gcd(0, 0) == 0 cannot be divided by.
        if (a == 0 || b == 0) return 0;
        return (a / greatestCommonDivisor(a, b)) * b;
    }


    template<typename Number>
    auto __attribute__((visibility("default"))) leastCommonMultiple(const std::vector <Number> &numbersIn) -> Number {
        Number lcm = 1UL;

        for (auto n: numbersIn) {
            lcm = leastCommonMultiple(lcm, n);
        }
        return lcm;
    }
}
#endif //LCM_GCD_HPP
