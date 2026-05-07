#include <boost/test/unit_test.hpp>
#include "isqrt.hpp"


BOOST_AUTO_TEST_CASE(test_isqrt) {
    for (unsigned int trial = 0; trial <= 33; ++trial) {
        const auto y = utility::isqrt(trial);
        BOOST_CHECK(y * y <= trial && (y + 1) * (y + 1) > trial);
    }
}

