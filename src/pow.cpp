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
auto utility::powmod<std::uint8_t>(const std::uint8_t base, const std::uint8_t exponent, const std::uint8_t& modulus) -> std::uint8_t {
    return powmodWith(base, exponent, modulus, wideMulmod<std::uint8_t, std::uint32_t>);
}


template <>
auto utility::powmod<std::uint16_t>(const std::uint16_t base, const std::uint16_t exponent, const std::uint16_t& modulus) -> std::uint16_t {
    return powmodWith(base, exponent, modulus, wideMulmod<std::uint16_t, std::uint32_t>);
}


template <>
auto utility::powmod<std::uint32_t>(const std::uint32_t base, const std::uint32_t exponent, const std::uint32_t& modulus) -> std::uint32_t {
    return powmodWith(base, exponent, modulus, wideMulmod<std::uint32_t, std::uint64_t>);
}


template <>
auto utility::powmod<std::uint64_t>(const std::uint64_t base, const std::uint64_t exponent, const std::uint64_t& modulus) -> std::uint64_t {
    return powmodWith(base, exponent, modulus, wideMulmod<std::uint64_t, uint128>);
}


template <>
auto utility::powmod<uint128>(const uint128 base, const uint128 exponent, const uint128& modulus) -> uint128 {
    return powmodWith(base, exponent, modulus, mulmod128);
}
