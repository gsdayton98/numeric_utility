// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2025 Glen S. Dayton. Rights reserved according to terms of included license.
#ifndef NUMERIC_UTILITY_POW_HPP
#define NUMERIC_UTILITY_POW_HPP
#include <cstdint>
#include <optional>
#include <type_traits>
#include "concepts.hpp"
#include <numeric_utility/numeric_utility_export.h>
/// @file
/// @brief Integer powers, plain and modular.
///
/// For cpp_int arguments, pow_multiprecision.hpp adds a powmod overload.

namespace utility {

    /**
     * @brief Evaluate base**exponent by repeated squaring.
     *
     * pow(0, 0) is 1, as for the empty product, and pow(0, n) is 0 for n > 0.
     * Beware, that the result is explicitly modulo the size of the unsigned type, for example,
     * base 32-bit unsigned int is evaluated modulo 2**32. Takes O(log exponent) multiplications.
     *
     * @code
     * utility::pow(3u, 4u);   // 81
     * @endcode
     * @tparam BaseType An integer type whose arithmetic wraps, such as the unsigned types.
     * @param base The base. Must be unsigned or at least non-negative.
     * @param exponent The exponent, of the same type. Must be unsigned or at least non-negative.
     * @return base raised to exponent, modulo the size of BaseType.
     */
    template<ModuloOverflow BaseType>
    auto pow(BaseType base, BaseType exponent) -> BaseType {
        // Types narrower than int promote to int, whose overflow is undefined, so multiply in unsigned.
        using Wide = std::common_type_t<BaseType, unsigned int>;
        BaseType result = 1;

        while (exponent > 0) {
            if (exponent & 1) result = static_cast<BaseType>(static_cast<Wide>(result) * base);
            base = static_cast<BaseType>(static_cast<Wide>(base) * base);
            exponent >>= 1;
        }

        return result;
    }


    /**
     * @brief Evaluate base**exponent modulo modulus.
     *
     * Beware, the generic version overflows unless modulus**2 fits in BaseType.
     * The specializations below hold for any modulus the type can hold.
     * Takes O(log exponent) modular multiplications.
     *
     * @code
     * utility::powmod<std::uint64_t>(2, 100, 1'000'000'007);   // 976371285
     * @endcode
     * @tparam BaseType An unsigned integer type.
     * @param base The base, which need not be less than modulus.
     * @param exponent The exponent.
     * @param modulus The modulus.
     * @return base**exponent mod modulus, which is 0 if modulus is 0 or 1.
     */
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

    /**
     * Multiply two integers, reporting overflow instead of wrapping.
     * @tparam T An integer type.
     * @param a The first factor.
     * @param b The second factor.
     * @return a * b, or std::nullopt if it doesn't fit in T.
     */
    template <typename T>
    std::optional<T> checkedMul(T a, T b) {
        T result;
        if (__builtin_mul_overflow(a, b, &result)) return std::nullopt;
        return result;
    }

    // Specializations defined in pow.cpp. They must be declared here, before any use,
    // or callers silently instantiate the generic version instead.
    // Unlike the templates above, they are compiled into the library, which hides symbols by default,
    // so they must be exported explicitly.
    // Specializations match exact types, not widths: of unsigned long and unsigned long long,
    // only the one that is uint64_t on the platform is specialized.
    /// powmod for std::uint8_t, correct for every modulus the type can hold. See the generic powmod.
    /// @param base The base.
    /// @param exponent The exponent.
    /// @param modulus The modulus.
    /// @return base**exponent mod modulus.
    template<>
    NUMERIC_UTILITY_API auto powmod<std::uint8_t>(std::uint8_t base, std::uint8_t exponent, const std::uint8_t& modulus) -> std::uint8_t;

    /// powmod for std::uint16_t, correct for every modulus the type can hold. See the generic powmod.
    /// @param base The base.
    /// @param exponent The exponent.
    /// @param modulus The modulus.
    /// @return base**exponent mod modulus.
    template<>
    NUMERIC_UTILITY_API auto powmod<std::uint16_t>(std::uint16_t base, std::uint16_t exponent, const std::uint16_t& modulus) -> std::uint16_t;

    /// powmod for std::uint32_t, correct for every modulus the type can hold. See the generic powmod.
    /// @param base The base.
    /// @param exponent The exponent.
    /// @param modulus The modulus.
    /// @return base**exponent mod modulus.
    template<>
    NUMERIC_UTILITY_API auto powmod<std::uint32_t>(std::uint32_t base, std::uint32_t exponent, const std::uint32_t& modulus) -> std::uint32_t;

    /// powmod for std::uint64_t, correct for every modulus the type can hold. See the generic powmod.
    /// @param base The base.
    /// @param exponent The exponent.
    /// @param modulus The modulus.
    /// @return base**exponent mod modulus.
    template<>
    NUMERIC_UTILITY_API auto powmod<std::uint64_t>(std::uint64_t base, std::uint64_t exponent, const std::uint64_t& modulus) -> std::uint64_t;

    /// powmod for unsigned __int128, correct for every modulus the type can hold. See the generic powmod.
    /// @param base The base.
    /// @param exponent The exponent.
    /// @param modulus The modulus.
    /// @return base**exponent mod modulus.
    template<>
    NUMERIC_UTILITY_API auto powmod<unsigned __int128>(unsigned __int128 base, unsigned __int128 exponent, const unsigned __int128 &modulus) -> unsigned __int128;
}
#endif
