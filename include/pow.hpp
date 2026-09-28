// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2025 Glen S. Dayton. Rights reserved according to terms of included license.
#ifndef POW_HPP
#define POW_HPP
#include <cstdint>
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


    namespace detail {
        /// (left + right) mod modulus without overflow, given left, right < modulus.
        template<Unsigned T>
        auto addmod(const T& left, const T& right, const T& modulus) -> T {
            const T gap = modulus - right;
            return left >= gap ? left - gap : left + right;
        }

        /// (left * right) mod modulus without overflow, given left, right < modulus.
        template<Unsigned T>
        auto mulmod(const T& left, const T& right, const T& modulus) -> T {
            if constexpr (std::numeric_limits<T>::is_integer && !std::numeric_limits<T>::is_bounded) {
                // Arbitrary precision: the product cannot overflow.
                return left * right % modulus;
            } else if constexpr (sizeof(T) <= sizeof(std::uint32_t)) {
                // Also avoids promotion of narrow types to signed int, whose overflow is undefined.
                return static_cast<T>(static_cast<std::uint64_t>(left) * right % modulus);
            } else if constexpr (sizeof(T) <= sizeof(std::uint64_t)) {
                return static_cast<T>(static_cast<unsigned __int128>(left) * right % modulus);
            } else {
                // No wider type to multiply in: double and add, staying below the modulus.
                T result = 0;
                T addend = left;
                T multiplier = right;
                while (multiplier > 0) {
                    if (multiplier & 1) result = addmod(result, addend, modulus);
                    addend = addmod(addend, addend, modulus);
                    multiplier >>= 1;
                }
                return result;
            }
        }
    }


    ////
    /// powmod(base, exponent, modulus)
    /// Evaluate base**exponent modulo modulus, for any modulus the type can hold.
    /// This version should only be used with unsigned types.
    template<Unsigned BaseType>
    auto __attribute__((visibility("default"))) powmod(BaseType base, BaseType exponent, const BaseType& modulus) -> BaseType {
        if (modulus < 2) return 0;
        BaseType result = 1;
        base %= modulus;
        while (exponent > 0)
        {
            if (exponent & 1) result = detail::mulmod(result, base, modulus);
            base = detail::mulmod(base, base, modulus);
            exponent >>= 1;
        }
        return result;
    }
}
#endif
