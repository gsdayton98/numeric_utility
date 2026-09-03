// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2022 Glen S. Dayton. Rights reserved according to terms of included license.
#include <boost/test/unit_test.hpp>
#include "isqrt.hpp"
BOOST_AUTO_TEST_SUITE(TestISqrt)

BOOST_AUTO_TEST_CASE(test_isqrt) {
    for (unsigned int trial = 0; trial <= 33; ++trial) {
        const auto y = utility::isqrt(trial);
        BOOST_CHECK(y * y <= trial && (y + 1) * (y + 1) > trial);
    }
}
BOOST_AUTO_TEST_SUITE_END()
