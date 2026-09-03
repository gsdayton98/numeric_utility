// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2022 Glen S. Dayton. Rights reserved according to terms of included license.
#ifndef ISQRT_HPP
#define ISQRT_HPP
#include <type_traits>

namespace utility {
    template<typename NumberType>
    requires std::is_integral_v<NumberType> && std::is_unsigned_v<NumberType>
    auto __attribute__((visibility("default"))) isqrt(const NumberType &c) -> NumberType {
        auto x = c / 2;

        while (!(x * x <= c && (x + 1) * (x + 1) > c)) {
            auto y = x * x - c;
            if (x != 0) {
                auto x2 = 2 * x;
                x = x - (y + x2 - 1) / x2;
            } else
                x = 1;
        }
        return x;
    }
}
#endif //ISQRT_HPP
