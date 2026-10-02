// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2026 Glen S. Dayton. Rights reserved according to terms of included license.
#include <boost/test/unit_test.hpp>
#include <cstdint>
#include <numeric>
#include <optional>
#include <stdexcept>
#include <numeric_utility/totient.hpp>

BOOST_AUTO_TEST_SUITE(TestTotient)


BOOST_AUTO_TEST_CASE(test_small_values) {
    // OEIS A000010, φ(1) through φ(32).
    constexpr unsigned int expected[] = {
        1, 1, 2, 2, 4, 2, 6, 4, 6, 4, 10, 4, 12, 6, 8, 8,
        16, 6, 18, 8, 12, 10, 22, 8, 20, 12, 18, 12, 28, 8, 30, 16};
    const utility::Totient<unsigned int> totient(32);

    BOOST_CHECK_EQUAL(totient.size(), 33u);
    BOOST_CHECK_EQUAL(totient.phi(0), 0u);
    for (unsigned int n = 1; n <= 32; ++n) {
        BOOST_TEST(totient.phi(n) == expected[n - 1], "phi(" << n << ")");
        BOOST_TEST(totient[n] == expected[n - 1], "[" << n << "]");
    }
}


BOOST_AUTO_TEST_CASE(test_matches_definition) {
    // Count 1 <= k <= n coprime to n directly.
    constexpr unsigned int limit = 1'000;
    const utility::Totient<unsigned int> totient(limit);
    std::optional<unsigned int> mismatch;
    for (unsigned int n = 1; n <= limit && !mismatch; ++n) {
        unsigned int count = 0;
        for (unsigned int k = 1; k <= n; ++k) {
            if (std::gcd(k, n) == 1) ++count;
        }
        if (totient.phi(n) != count) mismatch = n;
    }
    BOOST_TEST(!mismatch.has_value(), "phi disagrees with the definition at " << mismatch.value_or(0));
}


BOOST_AUTO_TEST_CASE(test_sum) {
    // The number of reduced proper fractions with denominator at most 1,000,000 (Project Euler 72).
    const utility::Totient<unsigned int> totient(1'000'000);
    const auto& phi = totient.values();
    const auto sum = std::accumulate(phi.cbegin() + 2, phi.cend(), std::uint64_t{0});
    BOOST_CHECK_EQUAL(sum, 303'963'552'391ull);
}


BOOST_AUTO_TEST_CASE(test_limits) {
    const utility::Totient<unsigned int> totient(10);
    BOOST_CHECK_EQUAL(totient.phi(10), 4u);
    BOOST_CHECK_THROW(static_cast<void>(totient.phi(11)), std::range_error);

    // The whole range of a small type, which is where the sieve's last multiple would overflow.
    const utility::Totient<std::uint8_t> small(254);
    BOOST_CHECK_EQUAL(small.phi(254), 126u);    // 254 = 2 × 127
    BOOST_CHECK_EQUAL(small.phi(251), 250u);    // the largest 8-bit prime
    BOOST_CHECK_THROW(utility::Totient<std::uint8_t>(255), std::length_error);

    const utility::Totient<unsigned int> zero(0);
    BOOST_CHECK_EQUAL(zero.size(), 1u);
    BOOST_CHECK_EQUAL(zero.phi(0), 0u);
}

BOOST_AUTO_TEST_SUITE_END()
