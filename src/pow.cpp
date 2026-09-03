// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-
// Copyright 2025 Glen S. Dayton. Rights reserved according to terms of included license.
#include "pow.hpp"

namespace {
    auto mulmod(const unsigned int left, const unsigned int right, const unsigned int modulus) -> unsigned int {
        unsigned long product = left;
        product *= right;
        return product % modulus;
    }
}

template <>
auto utility::powmod<unsigned long>(unsigned long base, unsigned long exponent, const unsigned long &modulus) -> unsigned long {
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
auto utility::powmod<unsigned int>(unsigned int base, unsigned int exponent, const unsigned int& modulus) -> unsigned int;
