// -*- mode: c++ ;
//
// Created by Glen Dayton, new account on 11/12/22.
//

#ifndef ISQRT_HPP
#define ISQRT_HPP
namespace numutil {
    template<typename NumberType>
    requires std::is_integral_v<NumberType> && std::is_unsigned_v<NumberType>
    auto isqrt(const NumberType &c) -> NumberType {
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
