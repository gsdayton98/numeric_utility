//
// Created by Glen Dayton, new account on 3/23/26.
//
#include <boost/test/unit_test.hpp>
#include "amicable_numbers.hpp"
#include "factor.hpp"
using namespace utility;


BOOST_AUTO_TEST_CASE(test_amicable_numbers_d) {
    BOOST_CHECK_EQUAL(AmicableNumbers::d(220U), 284U);
}


BOOST_AUTO_TEST_CASE(test_amicable_numbers_divisors) {
    BOOST_CHECK_EQUAL(AmicableNumbers::divisors(220U, Factor::factor(220U)).size(), 11U);
}
