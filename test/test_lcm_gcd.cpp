// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2025 Glen S. Dayton. Rights reserved according to terms of included license.
//
// Created by Glen Dayton, new account on 8/29/25.
//
#include <boost/test/unit_test.hpp>
#include <cstdint>
#include <numeric>
#include <stdexcept>
#include <vector>
#include "lcm_gcd.hpp"
BOOST_AUTO_TEST_SUITE(TestLCM_GCD)

BOOST_AUTO_TEST_CASE(test_gcd)
{
    BOOST_CHECK_EQUAL(utility::greatestCommonDivisor(12, 18), 6);
    BOOST_CHECK_EQUAL(utility::greatestCommonDivisor(18, 12), 6);
    BOOST_CHECK_EQUAL(utility::greatestCommonDivisor(18, 0), 18);
    BOOST_CHECK_EQUAL(utility::greatestCommonDivisor(0, 12), 12);
    BOOST_CHECK_EQUAL(utility::greatestCommonDivisor(18, 1), 1);
}


BOOST_AUTO_TEST_CASE(test_lcm)
{
    BOOST_CHECK_EQUAL(utility::leastCommonMultiple(12, 18), 36);
    BOOST_CHECK_EQUAL(utility::leastCommonMultiple(18, 12), 36);
    BOOST_CHECK_EQUAL(utility::leastCommonMultiple(18, 0), 0);
    BOOST_CHECK_EQUAL(utility::leastCommonMultiple(0, 18), 0);
    BOOST_CHECK_EQUAL(utility::leastCommonMultiple(1, 18), 18);
    BOOST_CHECK_EQUAL(utility::leastCommonMultiple(18, 1), 18);
    BOOST_CHECK_EQUAL(utility::leastCommonMultiple(0, 0), 0);        // Divided by gcd(0, 0) == 0
    BOOST_CHECK_EQUAL(utility::leastCommonMultiple(0u, 0u), 0u);
}

BOOST_AUTO_TEST_CASE(test_vector_lcm_zeros)
{
    BOOST_CHECK_EQUAL(utility::leastCommonMultiple(std::vector{3, 0, 5}), 0);
    BOOST_CHECK_EQUAL(utility::leastCommonMultiple(std::vector{3, 0, 0}), 0);  // Running lcm 0 meets another 0
    BOOST_CHECK_EQUAL(utility::leastCommonMultiple(std::vector<int>{}), 1);    // Empty product
}

BOOST_AUTO_TEST_CASE(test_vector_lcm)
{
    const std::vector v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    BOOST_CHECK_EQUAL(utility::leastCommonMultiple(v), 2520);
}

BOOST_AUTO_TEST_CASE(test_lcm_overflow)
{
    // Results that fit, up to the type's maximum.
    BOOST_CHECK_EQUAL(utility::leastCommonMultiple(65'535u, 65'537u), 4'294'967'295u);
    BOOST_CHECK_EQUAL(utility::leastCommonMultiple(2'147'483'648u, 2'147'483'648u), 2'147'483'648u);
    BOOST_CHECK_EQUAL(utility::leastCommonMultiple(46'341, 46'340), 2'147'441'940);

    // Results that don't.
    BOOST_CHECK_THROW(utility::leastCommonMultiple(65'536u, 65'537u), std::overflow_error);
    BOOST_CHECK_THROW(utility::leastCommonMultiple(46'341, 46'342), std::overflow_error);
    BOOST_CHECK_THROW(utility::leastCommonMultiple(std::uint64_t{1} << 32, (std::uint64_t{1} << 32) + 1), std::overflow_error);
}

BOOST_AUTO_TEST_CASE(test_vector_lcm_overflow)
{
    std::vector<std::uint64_t> numbers(46);
    std::iota(numbers.begin(), numbers.end(), std::uint64_t{1});
    BOOST_CHECK_EQUAL(utility::leastCommonMultiple(numbers), 9'419'588'158'802'421'600u);  // lcm(1..46)

    numbers.push_back(47);
    BOOST_CHECK_THROW(utility::leastCommonMultiple(numbers), std::overflow_error);        // lcm(1..47) > 2^64
}

BOOST_AUTO_TEST_SUITE_END()