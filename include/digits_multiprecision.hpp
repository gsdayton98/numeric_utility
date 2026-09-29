// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2026 Glen S. Dayton. Rights reserved according to terms of included license.
#ifndef DIGITS_MULTIPRECISION_HPP
#define DIGITS_MULTIPRECISION_HPP
#include <boost/multiprecision/cpp_int.hpp>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <vector>
#include "digits.hpp"

// Kept apart from digits.hpp so only users of Boost.Multiprecision depend on it.
namespace utility {

    /**
     * Return a vector of digits representing the number n in the given base.
     * Each multiprecision division peels off as many digits as fit in 64 bits,
     * rather than one digit at a time.
     * @tparam RadixType Type of the base
     * @tparam DigitType Type of the digits in the result
     * @param n The non-negative number to convert
     * @param base The base to convert to, at least 2
     * @return A vector of digits, least significant first, representing the number in the given base
     * @throw std::domain_error if n is negative or base is less than 2.
     */
    template <Unsigned RadixType = DefaultRadixType, Unsigned DigitType = DefaultDigitType>
    requires (sizeof(RadixType) <= sizeof(std::uint64_t))
    auto toDigits(const boost::multiprecision::cpp_int& n, RadixType base = 10u) -> std::vector<DigitType>
    {
        using Limb = std::uint64_t;
        using boost::multiprecision::cpp_int;

        if (n < 0) throw std::domain_error("toDigits: number is negative");
        if (base < 2) throw std::domain_error("toDigits: base is less than 2");

        // Find the largest power of the base that fits in a Limb.
        const Limb radix = base;
        Limb chunk = radix;
        unsigned int width = 1;
        while (chunk <= std::numeric_limits<Limb>::max() / radix) {
            chunk *= radix;
            ++width;
        }
        const cpp_int divisor = chunk;

        std::vector<DigitType> result;
        cpp_int quotient = n;
        cpp_int nextQuotient;
        cpp_int remainder;
        do {
            boost::multiprecision::divide_qr(quotient, divisor, nextQuotient, remainder);
            quotient.swap(nextQuotient);
            auto low = remainder.convert_to<Limb>();
            if (quotient == 0) {
                // Most significant chunk: omit leading zeros.
                do {
                    result.push_back(static_cast<DigitType>(low % radix));
                    low /= radix;
                } while (low != 0);
            } else {
                for (unsigned int i = 0; i < width; ++i) {
                    result.push_back(static_cast<DigitType>(low % radix));
                    low /= radix;
                }
            }
        } while (quotient != 0);

        return result;
    }
}

#endif //DIGITS_MULTIPRECISION_HPP
