//
// Created by Glen Dayton, new account on 3/13/24.
//
#include <boost/test/unit_test.hpp>
#include "digits.hpp"
BOOST_AUTO_TEST_CASE(test_digits)
{
    int sample = 76543210;
    int expected[] = {0, 1, 2, 3, 4, 5, 6, 7};

    auto result = toDigits(sample, 10);
    BOOST_CHECK_EQUAL(result.size(), 8);
    BOOST_CHECK_EQUAL(expected[0], 0);
    BOOST_CHECK_EQUAL(expected[1], 1);
    BOOST_CHECK_EQUAL(expected[2], 2);
    BOOST_CHECK_EQUAL(expected[3], 3);
    BOOST_CHECK_EQUAL(expected[4], 4);
    BOOST_CHECK_EQUAL(expected[5], 5);
    BOOST_CHECK_EQUAL(expected[6], 6);
    BOOST_CHECK_EQUAL(expected[7], 7);
}