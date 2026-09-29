// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2026 Glen S. Dayton. Rights reserved according to terms of included license.
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/test/unit_test.hpp>
#include <cstdint>
#include <string>
#include <vector>
#include "concepts.hpp"
#include "digits.hpp"
#include "isqrt.hpp"

using utility::Unsigned;
using boost::multiprecision::cpp_int;
using boost::multiprecision::uint128_t;
using boost::multiprecision::uint256_t;

namespace {
    struct NotANumber {};
}

// Built-in unsigned integers, including the extension type unsigned __int128.
static_assert(Unsigned<unsigned char> && Unsigned<unsigned short> && Unsigned<unsigned int>
              && Unsigned<unsigned long> && Unsigned<unsigned long long> && Unsigned<unsigned __int128>);
static_assert(Unsigned<std::uint8_t> && Unsigned<std::uint64_t> && Unsigned<std::size_t>);
static_assert(Unsigned<const unsigned int>);

// Boost's fixed-width unsigned types, through their numeric_limits specializations.
static_assert(Unsigned<uint128_t> && Unsigned<uint256_t>);

// Signed and floating-point types, bool, and types numeric_limits doesn't know.
static_assert(! Unsigned<int> && ! Unsigned<long long> && ! Unsigned<__int128>);
static_assert(! Unsigned<float> && ! Unsigned<double>);
static_assert(! Unsigned<bool> && ! Unsigned<const bool>);
static_assert(! Unsigned<cpp_int>);
static_assert(! Unsigned<NotANumber> && ! Unsigned<std::string> && ! Unsigned<unsigned int*>);

BOOST_AUTO_TEST_SUITE(TestConcepts)

// Boost's fixed-width unsigned types used to be rejected by isqrt and toDigits.
BOOST_AUTO_TEST_CASE(test_boost_fixed_width) {
    const uint128_t big = (uint128_t{1} << 100) + 12345;
    BOOST_CHECK_EQUAL(utility::isqrt(big), uint128_t{1} << 50);
    BOOST_CHECK_EQUAL(utility::isqrt(std::numeric_limits<uint128_t>::max()),
                      std::numeric_limits<std::uint64_t>::max());

    const std::vector<utility::DefaultDigitType> expected = {0, 1, 2, 3, 4, 5, 6, 7};
    const auto digits = utility::toDigits(uint128_t{76543210u});
    BOOST_CHECK_EQUAL_COLLECTIONS(digits.begin(), digits.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_SUITE_END()
