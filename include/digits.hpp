// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2024 Glen S. Dayton. Rights reserved according to terms of included license.
#ifndef TO_DIGITS_HPP
#define TO_DIGITS_HPP
#include <ranges>
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
            n /= base;
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
     */
    template <
        typename ResultType,
        typename DigitsType,
        typename RadixType=DefaultRadixType>
    requires std::ranges::forward_range<DigitsType>
    auto toNumber(const DigitsType& digits, RadixType base = 10u) -> ResultType
    {
        ResultType number = 0;

        for (auto digit: digits | std::views::reverse) {
            number = base*number + digit;
        }
        return number;
    }
}

#endif //TO_DIGITS_HPP
