// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2024 Glen S. Dayton. Rights reserved according to terms of included license.
#ifndef NUMERIC_UTILITY_DIGITS_HPP
#define NUMERIC_UTILITY_DIGITS_HPP
#include <ranges>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>
#include "concepts.hpp"
/// @file
/// @brief Conversion between unsigned integers and vectors of digits in any base.
///
/// For numbers too big for a built-in type, digits_multiprecision.hpp adds a toDigits overload for
/// Boost.Multiprecision's cpp_int.
namespace utility {

    /// The default type of a single digit.
    using DefaultDigitType = unsigned char;
    /// The default type of a base.
    using DefaultRadixType = unsigned int;
    /**
     * Return a vector of digits representing the number n in the given base.
     *
     * The least significant digit comes first, and zero is the single digit 0. The base must be at least 2: a
     * base of 0 divides by zero, and a base of 1 never terminates.
     *
     * @code
     * auto digits = utility::toDigits(1234u);        // {4, 3, 2, 1}
     * auto bits   = utility::toDigits(10u, 2u);      // {0, 1, 0, 1}
     * @endcode
     * @tparam Number Type of the input number
     * @tparam RadixType Type of the base
     * @tparam DigitType Type of the digits in the result
     * @param n The number to convert
     * @param base The base to convert to
     * @return A vector of digits, least significant first, representing the number in the given base
     */
    template <Unsigned Number, Unsigned RadixType = DefaultRadixType, Unsigned DigitType = DefaultDigitType>
    auto toDigits(Number n, RadixType base = 10u) -> std::vector<DigitType>
    {
        std::vector<DigitType> result;
        do {
            result.push_back(static_cast<DigitType>(n % base));
            n = static_cast<Number>(n / base);   // Narrow types promote to int, which -Wconversion flags.
        } while (n != 0);

        return result;
    }

    /**
     * Convert a vector of digits back to a number in the given base.
     *
     * The inverse of toDigits: the first digit is the least significant.
     *
     * @code
     * auto n = utility::toNumber<unsigned>(std::vector<unsigned char>{4, 3, 2, 1});   // 1234
     * @endcode
     * @tparam ResultType Type of the resulting number
     * @tparam DigitsType A forward range of digits, such as a std::vector
     * @tparam RadixType Type of the base
     * @param digits The digits to convert, least significant first
     * @param base The base of the digits
     * @return The number represented by the digits
     * @throw std::overflow_error if the result, the base or a digit does not fit in a built-in
     *        integer ResultType. Arbitrary-precision types cannot overflow.
     */
    template <
        typename ResultType,
        typename DigitsType,
        typename RadixType=DefaultRadixType>
    requires std::ranges::forward_range<DigitsType>
    auto toNumber(const DigitsType& digits, RadixType base = 10u) -> ResultType
    {
        ResultType number = 0;

        if constexpr (std::is_integral_v<ResultType>) {
            // Do all the arithmetic in ResultType, so signed and unsigned operands never mix.
            if (!std::in_range<ResultType>(base)) {
                throw std::overflow_error("toNumber: base does not fit in the type");
            }
            const auto radix = static_cast<ResultType>(base);
            for (auto digit: digits | std::views::reverse) {
                if (!std::in_range<ResultType>(digit)) {
                    throw std::overflow_error("toNumber: digit does not fit in the type");
                }
                if (__builtin_mul_overflow(number, radix, &number)
                    || __builtin_add_overflow(number, static_cast<ResultType>(digit), &number)) {
                    throw std::overflow_error("toNumber: result does not fit in the type");
                }
            }
        } else {
            for (auto digit: digits | std::views::reverse) {
                number = base*number + digit;
            }
        }
        return number;
    }
}

#endif //NUMERIC_UTILITY_DIGITS_HPP
