//
// Created by Glen Dayton, new account on 8/29/25.
//
#include <boost/test/unit_test.hpp>
#include "lcm_gcd.hpp"

BOOST_AUTO_TEST_CASE(test_gcd)
{
    BOOST_CHECK_EQUAL(numutil::greatestCommonDivisor(12, 18), 6);
    BOOST_CHECK_EQUAL(numutil::greatestCommonDivisor(18, 12), 6);
    BOOST_CHECK_EQUAL(numutil::greatestCommonDivisor(18, 0), 18);
    BOOST_CHECK_EQUAL(numutil::greatestCommonDivisor(0, 12), 12);
    BOOST_CHECK_EQUAL(numutil::greatestCommonDivisor(18, 1), 1);
}