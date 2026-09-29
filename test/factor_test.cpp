// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2022 Glen S. Dayton. Rights reserved according to terms of included license.
#include <boost/test/unit_test.hpp>
#include <cstddef>
#include <optional>
#include <random>
#include <sstream>
#include <vector>
#include "factor.hpp"
#include "miller_rabin.hpp"
using namespace utility;

namespace {
    // Factor's prime table is an implementation detail, and its former cache and lock must not come back.
    template <typename T> concept ExposesPrimes = requires { T::primes; };
    template <typename T> concept ExposesCache = requires { T::cache; };
    template <typename T> concept ExposesCacheLock = requires { T::cacheLock; };
    template <typename T> concept ExposesPreloadCache = requires { T::preloadCache(0u); };

    static_assert(!ExposesPrimes<Factor>);
    static_assert(!ExposesCache<Factor>);
    static_assert(!ExposesCacheLock<Factor>);
    static_assert(!ExposesPreloadCache<Factor>);
}

BOOST_AUTO_TEST_SUITE(TestFactor)

BOOST_AUTO_TEST_CASE(testFactorEvaluate) {
    const std::vector testCase12 {Factor {2,2}, Factor {3,1}};

    BOOST_CHECK_EQUAL(Factor::evaluate(testCase12), 12U);

    const std::vector testCase220 {Factor {2,2}, Factor{5,1}, Factor{11,1}};
    BOOST_CHECK_EQUAL(Factor::evaluate(testCase220), 220U);
}


BOOST_AUTO_TEST_CASE(testFactor) {
    const auto factors12 = Factor::factor(12U);
    BOOST_CHECK_EQUAL(factors12.size(), 2U);
    BOOST_CHECK( (factors12[0] == Factor{2, 2}) );
    BOOST_CHECK( (factors12[1] == Factor{3, 1}) );


    const auto factors220 = Factor::factor(220U);
    BOOST_CHECK_EQUAL(factors220.size(), 3U);
    BOOST_CHECK( (factors220[0] == Factor{2, 2}) );
    BOOST_CHECK( (factors220[1] == Factor{5, 1}) );
    BOOST_CHECK( (factors220[2] == Factor{11, 1}) );
}


BOOST_AUTO_TEST_CASE(testLargeNumber)
{
    unsigned int sample = 2'769'550'542u;
    const auto factors = Factor::factor(sample);
    BOOST_CHECK_EQUAL(factors.size(), 5u);
    BOOST_CHECK( (factors[0] == Factor{2, 1}) );
    BOOST_CHECK( (factors[1] == Factor{3, 4}) );
    BOOST_CHECK( (factors[2] == Factor{11, 1}) );
    BOOST_CHECK( (factors[3] == Factor{19, 1}) );
    BOOST_CHECK( (factors[4] == Factor{81799, 1}) );
}


BOOST_AUTO_TEST_CASE(testPrimePowerOverflow)
{
    // The next higher power of the prime does not fit in 32 bits.
    const auto factors2 = Factor::factor(1u << 31);
    BOOST_REQUIRE_EQUAL(factors2.size(), 1u);
    BOOST_CHECK( (factors2[0] == Factor{2, 31}) );

    const auto factors3 = Factor::factor(3'486'784'401u);   // 3^20
    BOOST_REQUIRE_EQUAL(factors3.size(), 1u);
    BOOST_CHECK( (factors3[0] == Factor{3, 20}) );

    const auto factors65521 = Factor::factor(65521u * 65521u);
    BOOST_REQUIRE_EQUAL(factors65521.size(), 1u);
    BOOST_CHECK( (factors65521[0] == Factor{65521, 2}) );
}


BOOST_AUTO_TEST_CASE(testLimits)
{
    const auto factorsMax = Factor::factor(0xFFFF'FFFFu);    // 3 * 5 * 17 * 257 * 65537
    BOOST_CHECK_EQUAL(Factor::evaluate(factorsMax), 0xFFFF'FFFFu);
    BOOST_CHECK_EQUAL(factorsMax.size(), 5u);

    const auto factorsPrime = Factor::factor(4'294'967'291u); // Largest 32-bit prime
    BOOST_REQUIRE_EQUAL(factorsPrime.size(), 1u);
    BOOST_CHECK( (factorsPrime[0] == Factor{4'294'967'291u, 1}) );
}


BOOST_AUTO_TEST_CASE(testOperators) {
    Factor leftOperand {2, 3};
    Factor rightOperand {5, 2};
    constexpr Factor anotherOperand {5, 3};
    constexpr Factor yetAnotherOperand {2, 3};

    BOOST_CHECK( (leftOperand == yetAnotherOperand) );
    BOOST_CHECK( (leftOperand != rightOperand) );
    BOOST_CHECK( (rightOperand != anotherOperand) );

    BOOST_CHECK( leftOperand < rightOperand );
    BOOST_CHECK( (rightOperand < anotherOperand) );
}


BOOST_AUTO_TEST_CASE(testInserter) {
    std::ostringstream output;
    constexpr Factor example {5, 2};
    output << example;
    BOOST_CHECK_EQUAL(output.str(), "5^2");
}

BOOST_AUTO_TEST_CASE(testRandom32Bit)
{
    // Each factorization multiplies back to n, with prime factors in strictly increasing order.
    std::mt19937 rng(12345);
    std::optional<unsigned int> wrong;
    for (int i = 0; i < 2'000 && !wrong; ++i) {
        const unsigned int n = rng();
        const auto factors = Factor::factor(n);
        bool ok = Factor::evaluate(factors) == n;
        for (std::size_t f = 0; ok && f < factors.size(); ++f) {
            ok = utility::millerRabin(factors[f].prime) && factors[f].exponent > 0
                 && (f == 0 || factors[f - 1].prime < factors[f].prime);
        }
        if (!ok) wrong = n;
    }
    BOOST_TEST(!wrong.has_value(), "factor(" << wrong.value_or(0) << ") is wrong");
}


BOOST_AUTO_TEST_CASE(testPrimeList)
{
    for (auto n=4u; n < 0xFFFFu; ++n) {
        auto factors = Factor::factor(n);
        if (auto result = Factor::evaluate(factors); n != result ) {
            BOOST_REQUIRE_EQUAL(n, result);
        }
    }
    BOOST_CHECK(true);
}
BOOST_AUTO_TEST_SUITE_END()