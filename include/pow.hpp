// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2025 Glen S. Dayton. Rights reserved according to terms of included license.
#ifndef POW_HPP
#define POW_HPP
#include <cstdint>
#include "concepts.hpp"

namespace utility {

    ////
    /// Pow(base, exponent)

    /// Evaluate base**exponent. base and exponent must be unsigned or at least non-negative.
    /// Beware, that the result is explicitly modulo the size of the unsigned type, for example,
    /// base 32-bit unsigned int is evaluated modulo 2**32.

    template<ModuloOverflow BaseType>
    auto pow(BaseType base, BaseType exponent) -> BaseType {
        BaseType result = 1;

        if (base != 0) {
            while (exponent > 0) {
                if (exponent & 1) result *= base;
                base *= base;
                exponent >>= 1;
            }
        }

        return result;
    }


    ////
    /// powmod(base, exponent, modulus)
    /// Evaluate base**exponent modulo modulus.
    /// This version should only be used with unsigned types.
    /// Beware, the generic version overflows unless modulus**2 fits in BaseType.
    /// The specializations below hold for any modulus the type can hold.
    template<Unsigned BaseType>
    auto powmod(BaseType base, BaseType exponent, const BaseType& modulus) -> BaseType {
        if (modulus < 2) return 0;
        BaseType result = 1;
        base %= modulus;
        while (exponent > 0)
        {
            if (exponent & 1) result = (result * base) % modulus;
            base = (base * base) % modulus;
            exponent >>= 1;
        }
        return result;
    }

    // Specializations defined in pow.cpp. They must be declared here, before any use,
    // or callers silently instantiate the generic version instead.
    // Unlike the templates above, they are compiled into the library, which hides symbols by default,
    // so they must be exported explicitly.
    // Specializations match exact types, not widths: of unsigned long and unsigned long long,
    // only the one that is uint64_t on the platform is specialized.
    template<>
    auto __attribute__((visibility("default"))) powmod<std::uint8_t>(std::uint8_t base, std::uint8_t exponent, const std::uint8_t& modulus) -> std::uint8_t;

    template<>
    auto __attribute__((visibility("default"))) powmod<std::uint16_t>(std::uint16_t base, std::uint16_t exponent, const std::uint16_t& modulus) -> std::uint16_t;

    template<>
    auto __attribute__((visibility("default"))) powmod<std::uint32_t>(std::uint32_t base, std::uint32_t exponent, const std::uint32_t& modulus) -> std::uint32_t;

    template<>
    auto __attribute__((visibility("default"))) powmod<std::uint64_t>(std::uint64_t base, std::uint64_t exponent, const std::uint64_t& modulus) -> std::uint64_t;

    template<>
    auto __attribute__((visibility("default"))) powmod<unsigned __int128>(unsigned __int128 base, unsigned __int128 exponent, const unsigned __int128 &modulus) -> unsigned __int128;
}
#endif
