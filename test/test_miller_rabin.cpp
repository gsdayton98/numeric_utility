#include <boost/test/unit_test.hpp>

#include "miller_rabin.hpp"

BOOST_AUTO_TEST_CASE(test_miller_rabin) {
    constexpr unsigned int sample = 65537u;

    const auto result = utility::millerRabin(sample);
    BOOST_CHECK(result);
}
