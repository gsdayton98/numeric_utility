// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2026 Glen S. Dayton. Rights reserved according to terms of included license.
#ifndef POW_MULTIPRECISION_HPP
#define POW_MULTIPRECISION_HPP
#include <boost/multiprecision/cpp_int.hpp>
#include "pow.hpp"

// Kept apart from pow.hpp so only users of Boost.Multiprecision depend on it.
namespace utility {

    ////
    /// powmod(base, exponent, modulus)
    /// Evaluate base**exponent modulo modulus.
    /// An overload rather than a specialization: cpp_int is signed, so it does not
    /// satisfy the utility::Unsigned constraint of the template.
    /// Arguments must be non-negative.
    inline auto powmod(const boost::multiprecision::cpp_int& base,
                       const boost::multiprecision::cpp_int& exponent,
                       const boost::multiprecision::cpp_int& modulus) -> boost::multiprecision::cpp_int {
        if (modulus < 2) return 0;
        return boost::multiprecision::powm(base, exponent, modulus);
    }
}

#endif //POW_MULTIPRECISION_HPP
