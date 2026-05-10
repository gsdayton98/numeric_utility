//
// Created by Glen Dayton, new account on 3/13/24.
//
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/test/unit_test.hpp>
#include "digits.hpp"

using namespace utility;

using namespace boost::multiprecision;
using BigNumber = cpp_int;

template<> struct is_unsigned<BigNumber> : std::true_type {};

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

BOOST_AUTO_TEST_CASE(test_number_multiprecision)
{
    const std::vector<DefaultDigitType> digits = {0, 1, 2, 3, 4, 5, 6, 7};
    const BigNumber expected = 76'543'210;
    const auto result = utility::toNumber<BigNumber>(digits);
    BOOST_CHECK_EQUAL(result, expected);
}


BOOST_AUTO_TEST_SUITE_END()