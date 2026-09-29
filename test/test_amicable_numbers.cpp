// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2026 Glen S. Dayton. Rights reserved according to terms of included license.
#include <boost/test/unit_test.hpp>
#include "amicable_numbers.hpp"
#include "factor.hpp"
using namespace utility;

BOOST_AUTO_TEST_SUITE(TestAmicableNumbers)
BOOST_AUTO_TEST_CASE(test_amicable_numbers_d) {
    BOOST_CHECK_EQUAL(AmicableNumbers::d(220U), 284U);
}


BOOST_AUTO_TEST_CASE(test_amicable_numbers_d_large) {
    // The sum of proper divisors can exceed 32 bits.
    BOOST_CHECK_EQUAL(AmicableNumbers::d(4'294'967'040U), 15'150'635'136U);  // 2^8 * (2^24 - 1)
    BOOST_CHECK_EQUAL(AmicableNumbers::d(3'491'888'400U), 15'106'921'200U);  // Highly composite
    BOOST_CHECK_EQUAL(AmicableNumbers::d(4'294'967'295U), 3'009'636'033U);   // 2^32 - 1
    BOOST_CHECK_EQUAL(AmicableNumbers::d(4'294'967'291U), 1U);               // Largest 32-bit prime
    BOOST_CHECK_EQUAL(AmicableNumbers::d(1U), 0U);
    BOOST_CHECK_EQUAL(AmicableNumbers::d(0U), 0U);
}


BOOST_AUTO_TEST_CASE(test_amicable_numbers_divisors) {
    BOOST_CHECK_EQUAL(AmicableNumbers::divisors(220U, Factor::factor(220U)).size(), 11U);
}
BOOST_AUTO_TEST_SUITE_END()