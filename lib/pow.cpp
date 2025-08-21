//
// Created by Glen Dayton on 3/5/25.
//
#include "numerical_utility.hpp"
#include "pow.hpp"

namespace {
    auto mulmod(const unsigned int left, const unsigned int right, const unsigned int modulus) -> unsigned int {
        unsigned long product = left;
        product *= right;
        return product % modulus;
    }
}

template <>
auto numutil::powmod<unsigned long>(unsigned long base, unsigned long exponent, const unsigned long &modulus) -> unsigned long {
    if (modulus < 2) return 0;
    unsigned int result = 1;
    base %= modulus;
    while (exponent > 0)
    {
        if (exponent & 1)
        {
            result = ::mulmod(result, base, modulus);
        }
        base = mulmod(base, base, modulus);
        exponent >>= 1;
    }
    return result;
}

template<>
auto numutil::powmod<unsigned int>(unsigned int base, unsigned int exponent, const unsigned int& modulus) -> unsigned int;
