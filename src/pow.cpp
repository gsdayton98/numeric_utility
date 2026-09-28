// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-
// Copyright 2025 Glen S. Dayton. Rights reserved according to terms of included license.
#include <cstdint>
#include "pow.hpp"

namespace {
    using uint128 = unsigned __int128;

    /// (left * right) mod modulus, multiplying in a type wide enough to hold the product.
    template <typename Number, typename Wide>
    auto wideMulmod(const Number left, const Number right, const Number modulus) -> Number {
        return static_cast<Number>(static_cast<Wide>(left) * right % modulus);
    }

    /// (left + right) mod modulus without overflow, given left, right < modulus.
    auto addmod(const uint128 left, const uint128 right, const uint128 modulus) -> uint128 {
        const uint128 gap = modulus - right;
        return left >= gap ? left - gap : left + right;
    }

    /// (left * right) mod modulus without overflow, given left, right < modulus.
    /// No wider type exists, so double and add.
    auto mulmod128(const uint128 left, uint128 right, const uint128 modulus) -> uint128 {
        uint128 result = 0;
        uint128 addend = left;
        while (right > 0) {
            if (right & 1) result = addmod(result, addend, modulus);
            addend = addmod(addend, addend, modulus);
            right >>= 1;
        }
        return result;
    }

    template <typename Number, typename MulMod>
    auto powmodWith(Number base, Number exponent, const Number modulus, MulMod mulmod) -> Number {
        if (modulus < 2) return 0;
        Number result = 1;
        base %= modulus;
        while (exponent > 0)
        {
            if (exponent & 1) result = mulmod(result, base, modulus);
            base = mulmod(base, base, modulus);
            exponent >>= 1;
        }
        return result;
    }
}


// Narrow types multiply in uint32_t: promotion to int would make the overflow undefined.
template <>
auto utility::powmod<unsigned char>(const unsigned char base, const unsigned char exponent, const unsigned char& modulus) -> unsigned char {
    return powmodWith(base, exponent, modulus, wideMulmod<unsigned char, std::uint32_t>);
}


template <>
auto utility::powmod<unsigned short>(const unsigned short base, const unsigned short exponent, const unsigned short& modulus) -> unsigned short {
    return powmodWith(base, exponent, modulus, wideMulmod<unsigned short, std::uint32_t>);
}


template <>
auto utility::powmod<unsigned int>(const unsigned int base, const unsigned int exponent, const unsigned int& modulus) -> unsigned int {
    return powmodWith(base, exponent, modulus, wideMulmod<unsigned int, std::uint64_t>);
}


template <>
auto utility::powmod<unsigned long>(const unsigned long base, const unsigned long exponent, const unsigned long& modulus) -> unsigned long {
    return powmodWith(base, exponent, modulus, wideMulmod<unsigned long, uint128>);
}


template <>
auto utility::powmod<unsigned long long>(const unsigned long long base, const unsigned long long exponent, const unsigned long long& modulus) -> unsigned long long {
    return powmodWith(base, exponent, modulus, wideMulmod<unsigned long long, uint128>);
}


template <>
auto utility::powmod<unsigned __int128>(const uint128 base, const uint128 exponent, const uint128& modulus) -> uint128 {
    return powmodWith(base, exponent, modulus, mulmod128);
}
