#include <boost/test/unit_test.hpp>
#include "isqrt.hpp"


BOOST_AUTO_TEST_CASE(test_isqrt) {
    for (int trial = 0; trial <= 65; ++trial) {
        const int y = numutil::isqrt(trial);
        BOOST_CHECK(y * y <= trial && (y + 1) * (y + 1) > trial);
    }
}

