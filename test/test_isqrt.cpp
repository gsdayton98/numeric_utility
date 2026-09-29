// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2022 Glen S. Dayton. Rights reserved according to terms of included license.
#include <boost/test/unit_test.hpp>
#include <cstdint>
#include <limits>
#include "isqrt.hpp"

namespace {
    using U128 = unsigned __int128;

    // The root, widened so it compares and prints as a number; every root here fits in 64 bits.
    template <typename Number>
    auto root(const Number n) -> std::uint64_t {
        return static_cast<std::uint64_t>(utility::isqrt(n));
    }
}

BOOST_AUTO_TEST_SUITE(TestISqrt)

BOOST_AUTO_TEST_CASE(test_isqrt) {
    for (unsigned int trial = 0; trial <= 33; ++trial) {
        const auto y = utility::isqrt(trial);
        BOOST_CHECK(y * y <= trial && (y + 1) * (y + 1) > trial);
    }
}

// For each width: both endpoints, and a perfect square k^2 near the middle of the range with its neighbours.

BOOST_AUTO_TEST_CASE(test_isqrt_8_bit) {
    using U8 = std::uint8_t;
    BOOST_CHECK_EQUAL(root(U8{0}), 0u);
    BOOST_CHECK_EQUAL(root(U8{120}), 10u);
    BOOST_CHECK_EQUAL(root(U8{121}), 11u);                                  // 11^2
    BOOST_CHECK_EQUAL(root(U8{122}), 11u);
    BOOST_CHECK_EQUAL(root(std::numeric_limits<U8>::max()), 15u);
}

BOOST_AUTO_TEST_CASE(test_isqrt_16_bit) {
    using U16 = std::uint16_t;
    BOOST_CHECK_EQUAL(root(U16{0}), 0u);
    BOOST_CHECK_EQUAL(root(U16{32'760}), 180u);
    BOOST_CHECK_EQUAL(root(U16{32'761}), 181u);                             // 181^2
    BOOST_CHECK_EQUAL(root(U16{32'762}), 181u);
    BOOST_CHECK_EQUAL(root(std::numeric_limits<U16>::max()), 255u);
}

BOOST_AUTO_TEST_CASE(test_isqrt_32_bit) {
    using U32 = std::uint32_t;
    BOOST_CHECK_EQUAL(root(U32{0}), 0u);
    BOOST_CHECK_EQUAL(root(U32{2'147'395'599u}), 46'339u);
    BOOST_CHECK_EQUAL(root(U32{2'147'395'600u}), 46'340u);                  // 46340^2
    BOOST_CHECK_EQUAL(root(U32{2'147'395'601u}), 46'340u);
    BOOST_CHECK_EQUAL(root(std::numeric_limits<U32>::max()), 65'535u);
    BOOST_CHECK_EQUAL(root(U32{4'294'901'760u}), 65'535u);                  // Hung before the overflow fix
}

BOOST_AUTO_TEST_CASE(test_isqrt_64_bit) {
    using U64 = std::uint64_t;
    BOOST_CHECK_EQUAL(root(U64{0}), 0u);
    BOOST_CHECK_EQUAL(root(U64{9'223'372'030'926'249'000u}), 3'037'000'498u);
    BOOST_CHECK_EQUAL(root(U64{9'223'372'030'926'249'001u}), 3'037'000'499u); // 3037000499^2
    BOOST_CHECK_EQUAL(root(U64{9'223'372'030'926'249'002u}), 3'037'000'499u);
    BOOST_CHECK_EQUAL(root(std::numeric_limits<U64>::max()), 4'294'967'295u);
}

BOOST_AUTO_TEST_CASE(test_isqrt_128_bit) {
    constexpr std::uint64_t k = 13'043'817'825'332'782'212u;               // isqrt((2^128 - 1) / 2)
    const U128 square = U128{k} * k;
    BOOST_CHECK_EQUAL(root(U128{0}), 0u);
    BOOST_CHECK_EQUAL(root(square - 1), k - 1);
    BOOST_CHECK_EQUAL(root(square), k);
    BOOST_CHECK_EQUAL(root(square + 1), k);
    BOOST_CHECK_EQUAL(root(std::numeric_limits<U128>::max()), std::numeric_limits<std::uint64_t>::max());
}

BOOST_AUTO_TEST_SUITE_END()
