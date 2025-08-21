#include <boost/test/unit_test.hpp>

#include "miller_rabin.hpp"

BOOST_AUTO_TEST_CASE(test_miller_rabin) {
    unsigned int sample = 65537u;

    auto result = millerRabin(sample);
    BOOST_CHECK(result);
}
