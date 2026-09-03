// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2025 Glen S. Dayton. Rights reserved according to terms of included license.

#include <boost/test/unit_test.hpp>

#include "miller_rabin.hpp"

BOOST_AUTO_TEST_SUITE(TestMillerRabin)


BOOST_AUTO_TEST_CASE(test_miller_rabin) {
    constexpr unsigned int sample = 65537u;

    const auto result = utility::millerRabin(sample);
    BOOST_CHECK(result);
}
BOOST_AUTO_TEST_SUITE_END()