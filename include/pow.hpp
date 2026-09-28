// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2025 Glen S. Dayton. Rights reserved according to terms of included license.
#ifndef POW_HPP
#define POW_HPP
#include <limits>

template <typename T>
concept ModuloOverflow = std::numeric_limits<T>::is_modulo;

template <typename T>
concept Unsigned = ! std::numeric_limits<T>::is_signed;


namespace utility {
    ////
    /// Pow(base, exponent)

    /// Evaluate base**exponent. base and exponent must be unsigned or at least non-negative.
    /// Beware, that the result is explicitly modulo the size of the unsigned type, for example,
    /// base 32-bit unsigned int is evaluated modulo 2**32.

    template<ModuloOverflow BaseType>
    BaseType __attribute__((visibility("default"))) pow(BaseType base, BaseType exponent) {
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
    auto __attribute__((visibility("default"))) powmod(BaseType base, BaseType exponent, const BaseType& modulus) -> BaseType {
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
    template<>
    auto __attribute__((visibility("default"))) powmod<unsigned int>(unsigned int base, unsigned int exponent, const unsigned int& modulus) -> unsigned int;

    template<>
    auto __attribute__((visibility("default"))) powmod<unsigned long>(unsigned long base, unsigned long exponent, const unsigned long& modulus) -> unsigned long;

    template<>
    auto __attribute__((visibility("default"))) powmod<unsigned __int128>(unsigned __int128 base, unsigned __int128 exponent, const unsigned __int128& modulus) -> unsigned __int128;
}
#endif
