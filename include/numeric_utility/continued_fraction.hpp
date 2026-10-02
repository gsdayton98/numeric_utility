// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2026 Glen S. Dayton. Rights reserved according to terms of included license.
#ifndef NUMERIC_UTILITY_CONTINUED_FRACTION_HPP
#define NUMERIC_UTILITY_CONTINUED_FRACTION_HPP
#include <vector>
#include "concepts.hpp"
#include "isqrt.hpp"


namespace utility {
    /**
     * The continued fraction of √n: [a0; a1, a2, ..., ap], where a1 ... ap is the repeating period.
     *
     * For example √23 = [4; 1, 3, 1, 8] and √2 = [1; 2]. The period always ends with 2·a0, and its length is
     * size() - 1. A perfect square has no period, so the result is just [a0].
     *
     * Uses the recurrence m' = d·a − m, d' = (n − m'²)/d, a' = (a0 + m')/d', which keeps every term between 0 and
     * 2·a0, so nothing overflows for any n the type can hold.
     * @param n The number whose square root to expand.
     * @return a0 followed by one period of the partial quotients.
     */
    template <Unsigned Number>
    auto sqrtContinuedFraction(const Number n) -> std::vector<Number> {
        const Number a0 = isqrt(n);
        std::vector<Number> terms{a0};
        if (n - a0 * a0 == 0) return terms;

        Number m = 0;
        Number d = 1;
        Number a = a0;
        while (a != 2 * a0) {
            m = static_cast<Number>(d * a - m);
            d = static_cast<Number>((n - m * m) / d);
            a = static_cast<Number>((a0 + m) / d);
            terms.push_back(a);
        }
        return terms;
    }
}

#endif //NUMERIC_UTILITY_CONTINUED_FRACTION_HPP
