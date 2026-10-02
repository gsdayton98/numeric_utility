// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2026 Glen S. Dayton. Rights reserved according to terms of included license.
#include <boost/test/unit_test.hpp>
#include <cstdint>
#include <limits>
#include <vector>
#include <numeric_utility/continued_fraction.hpp>

BOOST_AUTO_TEST_SUITE(TestContinuedFraction)

using Terms = std::vector<unsigned int>;


BOOST_AUTO_TEST_CASE(test_small_roots) {
    // The expansions listed in Project Euler 64's statement.
    const std::vector<std::pair<unsigned int, Terms>> expected{
        {2, {1, 2}},
        {3, {1, 1, 2}},
        {5, {2, 4}},
        {6, {2, 2, 4}},
        {7, {2, 1, 1, 1, 4}},
        {8, {2, 1, 4}},
        {10, {3, 6}},
        {11, {3, 3, 6}},
        {12, {3, 2, 6}},
        {13, {3, 1, 1, 1, 1, 6}},
        {23, {4, 1, 3, 1, 8}},
    };
    for (const auto& [n, terms] : expected) {
        const auto result = utility::sqrtContinuedFraction(n);
        BOOST_TEST(result == terms, "sqrt(" << n << ")");
    }
}


BOOST_AUTO_TEST_CASE(test_perfect_squares) {
    BOOST_TEST(utility::sqrtContinuedFraction(0u) == Terms{0});
    BOOST_TEST(utility::sqrtContinuedFraction(1u) == Terms{1});
    BOOST_TEST(utility::sqrtContinuedFraction(4u) == Terms{2});
    BOOST_TEST(utility::sqrtContinuedFraction(10'000u) == Terms{100});
}


BOOST_AUTO_TEST_CASE(test_odd_periods) {
    // "Exactly four continued fractions, for N <= 13, have an odd period" (Project Euler 64).
    unsigned int count = 0;
    for (unsigned int n = 2; n <= 13; ++n) {
        if ((utility::sqrtContinuedFraction(n).size() - 1) % 2 == 1) ++count;
    }
    BOOST_CHECK_EQUAL(count, 4u);
}


BOOST_AUTO_TEST_CASE(test_period_ends_with_twice_a0) {
    for (unsigned int n = 2; n <= 10'000; ++n) {
        const auto terms = utility::sqrtContinuedFraction(n);
        if (terms.size() == 1) continue;
        BOOST_TEST(terms.back() == 2 * terms.front(), "sqrt(" << n << ")");
    }
}


BOOST_AUTO_TEST_CASE(test_limits) {
    // a² + 1 = [a; 2a] and a² + 2a = [a; 1, 2a]. With a = 2^32 - 1, a² + 2a is 2^64 - 1, the largest uint64_t.
    constexpr std::uint64_t a = std::numeric_limits<std::uint32_t>::max();
    using Terms64 = std::vector<std::uint64_t>;
    BOOST_TEST(utility::sqrtContinuedFraction(a * a + 1) == (Terms64{a, 2 * a}));
    BOOST_TEST(utility::sqrtContinuedFraction(std::numeric_limits<std::uint64_t>::max()) == (Terms64{a, 1, 2 * a}));

    // The same at the top of an 8-bit type: 255 = 15² + 2·15.
    using Terms8 = std::vector<std::uint8_t>;
    BOOST_TEST(utility::sqrtContinuedFraction(std::uint8_t{255}) == (Terms8{15, 1, 30}));
}

BOOST_AUTO_TEST_SUITE_END()
