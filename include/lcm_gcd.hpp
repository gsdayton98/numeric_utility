// -*- mode: c++ ;
//
// Created by Glen Dayton, new account on 11/15/22.
//

#ifndef LCM_GCD_HPP
#define LCM_GCD_HPP
#include <vector>


namespace numutil {
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
