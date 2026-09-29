// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2024 Glen S. Dayton. Rights reserved according to terms of included license.
//
// Created by Glen Dayton, new account on 3/13/24.
//
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/test/unit_test.hpp>
#include <stdexcept>
#include "digits.hpp"
#include "digits_multiprecision.hpp"

using namespace utility;

using namespace boost::multiprecision;
using BigNumber = cpp_int;


BOOST_AUTO_TEST_SUITE(digits_tests)

BOOST_AUTO_TEST_CASE(test_digits_uint)
{
    constexpr unsigned int sample = 76543210u;
    const std::vector<DefaultDigitType> expected = {0, 1, 2, 3, 4, 5, 6, 7};

    const auto result = toDigits(sample);
    BOOST_REQUIRE_EQUAL(result.size(), expected.size());
    BOOST_CHECK_EQUAL_COLLECTIONS(result.begin(), result.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(test_digits_ushort)
{
    constexpr unsigned short int sample = 3210;
    const std::vector< DefaultDigitType> expected = {0, 1, 2, 3}; //NOLINT

    const auto result = toDigits(sample);
    BOOST_REQUIRE_EQUAL(result.size(), expected.size());
    BOOST_CHECK_EQUAL_COLLECTIONS(result.begin(), result.end(), expected.begin(), expected.end());
}

namespace {
    [[maybe_unused]]
    auto fromString(const std::string& str) -> BigNumber
    {
        std::istringstream sstr {str};
        BigNumber result;
        sstr >> result;
        return result;
    }
}


BOOST_AUTO_TEST_CASE(test_digits_multiprecision)
{
    const std::string bigNumberString = "987654321001234567899876543210"; //NOLINT
    const BigNumber sample = fromString(bigNumberString);
    const std::vector<DefaultDigitType> expected = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
        9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9
    };

    const auto result = toDigits(sample);
    BOOST_REQUIRE_EQUAL(result.size(), expected.size());
    BOOST_CHECK_EQUAL_COLLECTIONS(result.begin(), result.end(), expected.begin(), expected.end());
}

namespace {
    // One digit per division; the reference for the chunked multiprecision toDigits.
    template <typename DigitType = DefaultDigitType>
    auto referenceDigits(BigNumber n, unsigned long long base) -> std::vector<DigitType>
    {
        std::vector<DigitType> result;
        do {
            result.push_back(static_cast<DigitType>(n % base));
            n /= base;
        } while (n != 0);
        return result;
    }
}

BOOST_AUTO_TEST_CASE(test_digits_multiprecision_chunk_boundaries)
{
    // Around 10^19, the decimal chunk size, and multiples of it.
    for (const auto exponent: {1u, 18u, 19u, 20u, 38u, 57u, 100u}) {
        const BigNumber power = pow(BigNumber{10}, exponent);
        for (const BigNumber& sample: {BigNumber{power - 1}, power, BigNumber{power + 1}}) {
            const auto result = toDigits(sample);
            const auto expected = referenceDigits(sample, 10u);
            BOOST_CHECK_EQUAL_COLLECTIONS(result.begin(), result.end(), expected.begin(), expected.end());
            BOOST_CHECK_EQUAL(toNumber<BigNumber>(result), sample);
        }
    }
}

BOOST_AUTO_TEST_CASE(test_digits_multiprecision_zero)
{
    const auto result = toDigits(BigNumber{0});
    BOOST_REQUIRE_EQUAL(result.size(), 1u);
    BOOST_CHECK_EQUAL(result[0], 0u);
}

BOOST_AUTO_TEST_CASE(test_digits_multiprecision_bases)
{
    const BigNumber sample = pow(BigNumber{3}, 200) + 12345u;
    for (const auto base: {2u, 7u, 16u, 255u}) {
        const auto result = toDigits(sample, base);
        const auto expected = referenceDigits(sample, base);
        BOOST_CHECK_EQUAL_COLLECTIONS(result.begin(), result.end(), expected.begin(), expected.end());
    }

    // A base too big for the default digit type; each chunk holds a single digit.
    constexpr auto bigBase = 0xFFFF'FFFFu;
    const auto result = toDigits<unsigned int, unsigned int>(sample, bigBase);
    const auto expected = referenceDigits<unsigned int>(sample, bigBase);
    BOOST_CHECK_EQUAL_COLLECTIONS(result.begin(), result.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(test_digits_multiprecision_domain)
{
    BOOST_CHECK_THROW(toDigits(BigNumber{-1}), std::domain_error);
    BOOST_CHECK_THROW(toDigits(BigNumber{10}, 1u), std::domain_error);
    BOOST_CHECK_THROW(toDigits(BigNumber{10}, 0u), std::domain_error);
}

BOOST_AUTO_TEST_CASE(test_number_int)
{
    const std::vector<DefaultDigitType> digits = {0, 1, 2, 3, 4, 5, 6, 7};
    constexpr int expected = 76'543'210;
    const auto result = utility::toNumber<int>(digits);
    BOOST_CHECK_EQUAL(result, expected);
}

BOOST_AUTO_TEST_CASE(test_number_unsigned_int)
{
    const std::vector<DefaultDigitType> digits = {0, 1, 2, 3, 4, 5, 6, 7};
    constexpr unsigned int expected = 76'543'210u;
    const auto result = utility::toNumber<unsigned int>(digits);
    BOOST_CHECK_EQUAL(result, expected);
}

BOOST_AUTO_TEST_CASE(test_number_long)
{
    const std::vector<DefaultDigitType> digits = {0, 1, 2, 3, 4, 5, 6, 7};
    constexpr long expected = 76'543'210;
    const auto result = utility::toNumber<long>(digits);
    BOOST_CHECK_EQUAL(result, expected);
}

BOOST_AUTO_TEST_CASE(test_number_unsigned_long)
{
    const std::vector<DefaultDigitType> digits = {0, 1, 2, 3, 4, 5, 6, 7};
    constexpr unsigned long expected = 76'543'210u;
    const auto result = utility::toNumber<unsigned long>(digits);
    BOOST_CHECK_EQUAL(result, expected);
}


BOOST_AUTO_TEST_CASE(test_number_long_long)
{
    const std::vector<DefaultDigitType> digits = {0, 1, 2, 3, 4, 5, 6, 7};
    constexpr long long expected = 76'543'210;
    const auto result = utility::toNumber<long>(digits);
    BOOST_CHECK_EQUAL(result, expected);
}

BOOST_AUTO_TEST_CASE(test_number_unsigned_long_long)
{
    const std::vector<DefaultDigitType> digits = {0, 1, 2, 3, 4, 5, 6, 7};
    constexpr unsigned long long expected = 76'543'210u;
    const auto result = utility::toNumber<unsigned long long>(digits);
    BOOST_CHECK_EQUAL(result, expected);
}

BOOST_AUTO_TEST_CASE(test_number_at_limits)
{
    // 2147483647 and 4294967295, least significant digit first.
    BOOST_CHECK_EQUAL(utility::toNumber<int>(std::vector<DefaultDigitType>{7, 4, 6, 3, 8, 4, 7, 4, 1, 2}),
                      std::numeric_limits<int>::max());
    BOOST_CHECK_EQUAL(utility::toNumber<unsigned int>(std::vector<DefaultDigitType>{5, 9, 2, 7, 6, 9, 4, 9, 2, 4}),
                      std::numeric_limits<unsigned int>::max());
}

BOOST_AUTO_TEST_CASE(test_number_overflow)
{
    // 2147483648 and 4294967296.
    BOOST_CHECK_THROW(utility::toNumber<int>(std::vector<DefaultDigitType>{8, 4, 6, 3, 8, 4, 7, 4, 1, 2}),
                      std::overflow_error);
    BOOST_CHECK_THROW(utility::toNumber<unsigned int>(std::vector<DefaultDigitType>{6, 9, 2, 7, 6, 9, 4, 9, 2, 4}),
                      std::overflow_error);
    // 20 digits overflow even unsigned long long.
    BOOST_CHECK_THROW(utility::toNumber<unsigned long long>(std::vector<DefaultDigitType>(20, 9)),
                      std::overflow_error);
    // Digits and bases that don't fit the result type.
    BOOST_CHECK_THROW(utility::toNumber<signed char>(std::vector<DefaultDigitType>{200}), std::overflow_error);
    BOOST_CHECK_THROW(utility::toNumber<signed char>(std::vector<DefaultDigitType>{1}, 200u), std::overflow_error);
}

BOOST_AUTO_TEST_CASE(test_number_multiprecision_no_overflow)
{
    const BigNumber result = utility::toNumber<BigNumber>(std::vector<DefaultDigitType>(30, 9));
    BOOST_CHECK_EQUAL(result, BigNumber("999999999999999999999999999999"));
}

BOOST_AUTO_TEST_CASE(test_number_multiprecision)
{
    const std::vector<DefaultDigitType> digits = {0, 1, 2, 3, 4, 5, 6, 7};
    const BigNumber expected = 76'543'210;
    const auto result = utility::toNumber<BigNumber>(digits);
    BOOST_CHECK_EQUAL(result, expected);
}
BOOST_AUTO_TEST_SUITE_END()