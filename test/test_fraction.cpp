// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2026 Glen S. Dayton. Rights reserved according to terms of included license.
#include <boost/test/unit_test.hpp>
#include <cstdint>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <type_traits>
#include <numeric_utility/fraction.hpp>

BOOST_AUTO_TEST_SUITE(TestFraction)

using utility::Fraction;


BOOST_AUTO_TEST_CASE(test_reduce) {
    const Fraction fraction{3, 12};
    static_assert(std::is_same_v<decltype(fraction), const Fraction<int>>);
    BOOST_CHECK_EQUAL(fraction.numerator(), 1);
    BOOST_CHECK_EQUAL(fraction.denominator(), 4);
}


BOOST_AUTO_TEST_CASE(test_signs) {
    // The denominator is always positive; the numerator carries the sign.
    const Fraction negativeNumerator{-3, 12};
    BOOST_CHECK_EQUAL(negativeNumerator.numerator(), -1);
    BOOST_CHECK_EQUAL(negativeNumerator.denominator(), 4);

    const Fraction negativeDenominator{3, -12};
    BOOST_CHECK_EQUAL(negativeDenominator.numerator(), -1);
    BOOST_CHECK_EQUAL(negativeDenominator.denominator(), 4);
    BOOST_CHECK(negativeNumerator == negativeDenominator);

    BOOST_CHECK(Fraction(-3, -12) == Fraction(1, 4));
    BOOST_CHECK(Fraction(1, 4) != Fraction(-1, 4));
}


BOOST_AUTO_TEST_CASE(test_zero) {
    const Fraction zero{0, 5};
    BOOST_CHECK_EQUAL(zero.numerator(), 0);
    BOOST_CHECK_EQUAL(zero.denominator(), 1);
    BOOST_CHECK(zero == Fraction(0, -7));
    BOOST_CHECK(Fraction(3, 4) * zero == zero);
    BOOST_CHECK((Fraction(3, 4) * zero).denominator() == 1);

    BOOST_CHECK_THROW(Fraction(1, 0), std::domain_error);
}


BOOST_AUTO_TEST_CASE(test_arithmetic) {
    BOOST_CHECK(Fraction(2, 3) * Fraction(3, 4) == Fraction(1, 2));
    BOOST_CHECK(Fraction(-2, 3) * Fraction(3, -4) == Fraction(1, 2));
    BOOST_CHECK(Fraction(-2, 3) * Fraction(3, 4) == Fraction(-1, 2));

    // The four digit-cancelling fractions of Project Euler 33's statement.
    Fraction product{16, 64};
    product *= Fraction(19, 95);
    product *= Fraction(26, 65);
    product *= Fraction(49, 98);
    BOOST_CHECK(product == Fraction(1, 100));
}


BOOST_AUTO_TEST_CASE(test_print) {
    std::ostringstream out;
    out << Fraction(6, -8) << ' ' << Fraction(0, 3) << ' ' << Fraction<std::int8_t>(4, 6);
    BOOST_CHECK_EQUAL(out.str(), "-3/4 0/1 2/3");
}


BOOST_AUTO_TEST_CASE(test_limits) {
    constexpr int maximum = std::numeric_limits<int>::max();
    constexpr int minimum = std::numeric_limits<int>::min();

    BOOST_CHECK_THROW(Fraction(minimum, 1), std::overflow_error);
    BOOST_CHECK_THROW(Fraction(1, minimum), std::overflow_error);

    // Cancelling first keeps products in range whenever the reduced result fits.
    BOOST_CHECK(Fraction(maximum, 2) * Fraction(2, maximum) == Fraction(1, 1));
    // 2^31 - 1 is prime, so only the 3s cancel: the result is maximum/7, which fits.
    BOOST_CHECK(Fraction(maximum, 3) * Fraction(3, 7) == Fraction(maximum, 7));

    BOOST_CHECK_THROW(Fraction(maximum, 1) * Fraction(2, 1), std::overflow_error);
    BOOST_CHECK_THROW(Fraction(1, maximum) * Fraction(1, 2), std::overflow_error);
    // -2^30 * 2 is the minimum value, which a Fraction never holds.
    BOOST_CHECK_THROW(Fraction(-(1 << 30), 1) * Fraction(2, 1), std::overflow_error);

    // A narrow type works the same way.
    BOOST_CHECK(Fraction<std::int8_t>(100, 3) * Fraction<std::int8_t>(3, 100) == Fraction<std::int8_t>(1, 1));
    BOOST_CHECK_THROW(Fraction<std::int8_t>(100, 1) * Fraction<std::int8_t>(2, 1), std::overflow_error);
}

BOOST_AUTO_TEST_SUITE_END()
