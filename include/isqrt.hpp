// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2022 Glen S. Dayton. Rights reserved according to terms of included license.
#ifndef ISQRT_HPP
#define ISQRT_HPP
#include <type_traits>

namespace utility {
    /**
     * Integer square root: the largest y with y*y <= c.
     *
     * Newton's method from an initial guess at or above the root, so the iterates decrease
     * monotonically to it. With c < 2^b, the guess is 2^ceil(b/2); every iterate x then satisfies
     * x + c/x < 2^(ceil(b/2) + 1), so nothing overflows.
     */
    template<typename NumberType>
    requires std::is_integral_v<NumberType> && std::is_unsigned_v<NumberType>
    auto __attribute__((visibility("default"))) isqrt(const NumberType &c) -> NumberType {
        if (c < 2) return c;

        unsigned int bits = 0;
        for (NumberType rest = c; rest != 0; rest >>= 1) ++bits;

        auto x = static_cast<NumberType>(NumberType{1} << ((bits + 1) / 2));
        auto y = static_cast<NumberType>((x + c / x) / 2);
        while (y < x) {
            x = y;
            y = static_cast<NumberType>((x + c / x) / 2);
        }
        return x;
    }
}
#endif //ISQRT_HPP
