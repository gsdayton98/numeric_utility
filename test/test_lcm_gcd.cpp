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


BOOST_AUTO_TEST_CASE(test_lcm)
{
    BOOST_CHECK_EQUAL(numutil::leastCommonMultiple(12, 18), 36);
    BOOST_CHECK_EQUAL(numutil::leastCommonMultiple(18, 12), 36);
    BOOST_CHECK_EQUAL(numutil::leastCommonMultiple(18, 0), 0);
    BOOST_CHECK_EQUAL(numutil::leastCommonMultiple(0, 18), 0);
    BOOST_CHECK_EQUAL(numutil::leastCommonMultiple(1, 18), 18);
    BOOST_CHECK_EQUAL(numutil::leastCommonMultiple(18, 1), 18);
}

BOOST_AUTO_TEST_CASE(test_vector_lcm)
{
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    BOOST_CHECK_EQUAL(numutil::leastCommonMultiple(v), 2520);
}