// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2026 Glen S. Dayton. Rights reserved according to terms of included license.
#ifndef NUMERIC_UTILITY_POW_MULTIPRECISION_HPP
#define NUMERIC_UTILITY_POW_MULTIPRECISION_HPP
#include <boost/multiprecision/cpp_int.hpp>
#include "pow.hpp"
/// @file
/// @brief powmod for Boost.Multiprecision's cpp_int.

// Kept apart from pow.hpp so only users of Boost.Multiprecision depend on it.
namespace utility {

    /**
     * @brief Evaluate base**exponent modulo modulus, for arbitrary-precision numbers.
     *
     * An overload rather than a specialization: cpp_int is signed, so it does not
     * satisfy the utility::Unsigned constraint of the template.
     * Cannot overflow.
     *
     * @param base The base, non-negative.
     * @param exponent The exponent, non-negative.
     * @param modulus The modulus, non-negative.
     * @return base**exponent mod modulus, which is 0 if modulus is 0 or 1.
     */
    inline auto powmod(const boost::multiprecision::cpp_int& base,
                       const boost::multiprecision::cpp_int& exponent,
                       const boost::multiprecision::cpp_int& modulus) -> boost::multiprecision::cpp_int {
        if (modulus < 2) return 0;
        return boost::multiprecision::powm(base, exponent, modulus);
    }
}

#endif //NUMERIC_UTILITY_POW_MULTIPRECISION_HPP
