// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2022 Glen S. Dayton. Rights reserved according to terms of included license.
#include <boost/test/unit_test.hpp>
#include <cstdint>
#include <limits>
#include "isqrt.hpp"

namespace {
    using U128 = unsigned __int128;

    // y is the integer square root of n if y^2 <= n < (y + 1)^2, evaluated without overflow.
    template <typename Number>
    auto isRoot(const Number n, const Number y) -> bool {
        const U128 wideN = n;
        const U128 wideY = y;
        return wideY * wideY <= wideN && (wideY + 1) * (wideY + 1) > wideN;
    }

    // Check every n in [0, limit].
    template <typename Number>
    auto checkThrough(const Number limit) -> void {
        for (Number n = 0;; ++n) {
            const auto y = utility::isqrt(n);
            if (!isRoot(n, y)) {
                BOOST_ERROR("isqrt(" << static_cast<std::uint64_t>(n) << ") = " << static_cast<std::uint64_t>(y));
                return;
            }
            if (n == limit) break;
        }
    }

    // Check k^2 - 1, k^2 and k^2 + 1 for k in [first, last]; k^2 must fit in Number.
    template <typename Number>
    auto checkSquaresAround(const Number first, const Number last) -> void {
        for (Number k = first;; ++k) {
            const Number square = k * k;
            for (const Number n: {Number(square - 1), square, Number(square + 1)}) {
                if (!isRoot(n, utility::isqrt(n))) {
                    BOOST_ERROR("isqrt(" << static_cast<std::uint64_t>(n) << ") = "
                                         << static_cast<std::uint64_t>(utility::isqrt(n)));
                    return;
                }
            }
            if (k == last) break;
        }
    }
}

BOOST_AUTO_TEST_SUITE(TestISqrt)

BOOST_AUTO_TEST_CASE(test_isqrt) {
    for (unsigned int trial = 0; trial <= 33; ++trial) {
        const auto y = utility::isqrt(trial);
        BOOST_CHECK(y * y <= trial && (y + 1) * (y + 1) > trial);
    }
}

BOOST_AUTO_TEST_CASE(test_isqrt_every_8_and_16_bit) {
    checkThrough(std::numeric_limits<std::uint8_t>::max());
    checkThrough(std::numeric_limits<std::uint16_t>::max());
}

BOOST_AUTO_TEST_CASE(test_isqrt_32_bit_squares) {
    // Every perfect square in 32 bits, and its neighbours.
    checkSquaresAround<std::uint32_t>(1u, 65'535u);
}

BOOST_AUTO_TEST_CASE(test_isqrt_64_bit_squares) {
    checkSquaresAround<std::uint64_t>(1u, 100'000u);
    checkSquaresAround<std::uint64_t>(4'294'867'296u, 4'294'967'295u);  // Up to (2^32 - 1)^2
}

BOOST_AUTO_TEST_CASE(test_isqrt_maximums) {
    BOOST_CHECK_EQUAL(utility::isqrt(std::numeric_limits<std::uint8_t>::max()), 15u);
    BOOST_CHECK_EQUAL(utility::isqrt(std::numeric_limits<std::uint16_t>::max()), 255u);
    BOOST_CHECK_EQUAL(utility::isqrt(std::numeric_limits<std::uint32_t>::max()), 65'535u);
    BOOST_CHECK_EQUAL(utility::isqrt(4'294'901'760u), 65'535u);  // Hung before the fix
    BOOST_CHECK_EQUAL(utility::isqrt(std::numeric_limits<std::uint64_t>::max()), 4'294'967'295u);
    BOOST_CHECK(utility::isqrt(std::numeric_limits<U128>::max()) == std::numeric_limits<std::uint64_t>::max());
}

BOOST_AUTO_TEST_SUITE_END()
