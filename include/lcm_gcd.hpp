// -*- mode: c++ ;
//
// Created by Glen Dayton, new account on 11/15/22.
//

#ifndef LCM_GCD_HPP
#define LCM_GCD_HPP


namespace numutil {
    template<typename Number>
    auto greatestCommonDivisor(Number a, Number b) -> Number {
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
    [[maybe_unused]] auto leastCommonMultiple(Number a, Number b) -> Number {
        return (a / greatestCommonDivisor(a, b)) * b;
    }


    template<typename Number>
    // clang-tide check is in error — numbersIn is used in line 49.
#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedParameter"
    auto leastCommonMultiple(const std::vector <Number> &numbersIn) -> Number {
#pragma clang diagnostic pop
        Number lcm = 1UL;

        for (auto n: numbersIn) {
            lcm = leastCommonMultiple(lcm, n);
        }
        return lcm;
    }
}
#endif //LCM_GCD_HPP
