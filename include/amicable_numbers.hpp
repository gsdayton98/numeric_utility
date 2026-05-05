// -*- mode: c++ -*- ;
// ©2022 Glen S. Dayton. Project Euler has confidentiality rules, so do not copy nor publish this code.
#ifndef EULER21_AMICABLE_NUMBERS_AMICABLE_NUMBERS_HPP
#define EULER21_AMICABLE_NUMBERS_AMICABLE_NUMBERS_HPP
////
/// Let $d(n)$ be defined as the sum of proper divisors of $n$ (numbers less than $n$ which divide evenly into $n$).
/// If $d(a) = b$ and $d(b) = a$, where $a ≠ b$, then $a$ and $b$ are an amicable pair and $a$ and $b$ are called
/// *amicable numbers*.
///
/// For example, the proper divisors of $220$ are $1, 2, 4, 5, 10, 11, 20, 22, 44, 55$ and $110$;
/// therefore $d(220) = 284$. The proper divisors of $284$ are $1, 2, 4, 71$ and $142$; so $d(284) = 220$.

#include <vector>
#include "factor.hpp"

using Number = unsigned int;

class __attribute__((visibility("default"))) AmicableNumbers {
public:
    /// Sum the proper divisors of a number.
    static auto d(Number n) -> Number;

    /// Given the prime factors of a number, return all proper divisors of the number.
    static auto divisors(Number, const std::vector<Factor>& factors) -> std::vector<Number>;
};

#endif //EULER21_AMICABLE_NUMBERS_AMICABLE_NUMBERS_HPP