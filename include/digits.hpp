
//
// Created by Glen Dayton, new account on 3/13/24.
//

#ifndef TO_DIGITS_HPP
#define TO_DIGITS_HPP
#include <vector>
namespace utility {

    using DefaultDigitType = unsigned char;
    using DefaultRadixType = unsigned int;

    // C++ explicitly prohibits specialization of type traits for user-defined types,
    // so I define my own type traits for my template functions.
    template<typename T>
    struct is_unsigned : std::false_type{};

    template<typename T>
    requires std::is_arithmetic_v<T>
    struct is_unsigned<T> : std::is_unsigned<T>{};

    template<typename T>
        inline constexpr bool is_unsigned_v = is_unsigned<T>::value;
    /**
     * Return a vector of digits representing the number n in the given base.
     * @tparam Number Type of the input number
     * @tparam RadixType Type of the base
     * @tparam DigitType Type of the digits in the result
     * @param n The number to convert
     * @param base The base to convert to
     * @return A vector of digits representing the number in the given base
     */
    template <typename Number, typename RadixType=DefaultRadixType, typename DigitType = DefaultDigitType>
    requires is_unsigned_v<Number> && is_unsigned_v<RadixType> && is_unsigned_v<DigitType>
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
     * @tparam RadixType Type of the base
     * @tparam DigitType Type of the digits in the input vector
     * @param digits The vector of digits to convert
     * @param base The base of the digits
     * @return The number represented by the digits
     */
    template <typename ResultType, typename RadixType=DefaultRadixType, typename DigitType=DefaultDigitType>
    auto toNumber(const std::vector<DigitType>& digits, RadixType base = 10u) -> ResultType
    {
        ResultType number = 0;
        unsigned int power = 1;

        for (auto digit: digits) {
            number += digit*power;
            power *= base;
        }
        return number;
    }

#endif //TO_DIGITS_HPP
}
