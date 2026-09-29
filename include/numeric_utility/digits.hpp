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
namespace utility {

    using DefaultDigitType = unsigned char;
    using DefaultRadixType = unsigned int;
    /**
     * Return a vector of digits representing the number n in the given base.
     * @tparam Number Type of the input number
     * @tparam RadixType Type of the base
     * @tparam DigitType Type of the digits in the result
     * @param n The number to convert
     * @param base The base to convert to
     * @return A vector of digits representing the number in the given base
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
     * @tparam ResultType Type of the resulting number
     * @tparam DigitsType Type of the digits in the input vector
     * @tparam RadixType Type of the base
     * @param digits The vector of digits to convert
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
