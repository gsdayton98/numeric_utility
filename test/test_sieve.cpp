// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2022 Glen S. Dayton. Rights reserved according to terms of included license.
#include <boost/test/unit_test.hpp>
#include <cstdint>
#include <optional>
#include <stdexcept>
#include <vector>
#include "sieveprimes.hpp"
BOOST_AUTO_TEST_SUITE(TestSieve)
BOOST_AUTO_TEST_CASE(test_sieve) {
    using SieveType = utility::Sieve<unsigned int>;
    SieveType sieve(20);

    BOOST_CHECK(sieve.isPrime(2));
    BOOST_CHECK(sieve.isPrime(3));
    BOOST_CHECK(!sieve.isPrime(4));
    BOOST_CHECK(sieve.isPrime(5));
    BOOST_CHECK(!sieve.isPrime(6));
    BOOST_CHECK(sieve.isPrime(7));
    BOOST_CHECK(!sieve.isPrime(8));
    BOOST_CHECK(!sieve.isPrime(9));
    BOOST_CHECK(!sieve.isPrime(10));
    BOOST_CHECK(sieve.isPrime(11));
    BOOST_CHECK(!sieve.isPrime(12));
    BOOST_CHECK(sieve.isPrime(13));
    BOOST_CHECK(!sieve.isPrime(14));
    BOOST_CHECK(!sieve.isPrime(15));
    BOOST_CHECK(!sieve.isPrime(16));
    BOOST_CHECK(sieve.isPrime(17));
    BOOST_CHECK(!sieve.isPrime(18));
    BOOST_CHECK(sieve.isPrime(19));
    BOOST_CHECK(!sieve.isPrime(20));
    BOOST_CHECK(!sieve.isPrime(36));
    BOOST_CHECK(sieve.isPrime(73));
    BOOST_CHECK_THROW(sieve.isPrime(1001), std::runtime_error);
}


BOOST_AUTO_TEST_CASE(test_array_sieve) {
    using SieveType = utility::Sieve<unsigned int>;
    SieveType sieve(32);

    BOOST_CHECK_EQUAL(sieve[0], 2);
    BOOST_CHECK_EQUAL(sieve[1], 3);
    BOOST_CHECK_EQUAL(sieve[2], 5);
    BOOST_CHECK_EQUAL(sieve.size(), 11);
    BOOST_CHECK_EQUAL(sieve.last(), 31);
}


BOOST_AUTO_TEST_CASE(test_sieve_beyond_does_not_modify_primes) {
    const utility::Sieve<unsigned int> sieve(100);
    const std::vector<unsigned int> before = sieve.primes();

    BOOST_CHECK(sieve.isPrime(9'973));
    BOOST_CHECK(sieve.isPrime(9'973));
    BOOST_CHECK(!sieve.isPrime(9'991));                                 // 97 * 103

    BOOST_CHECK_EQUAL_COLLECTIONS(sieve.primes().begin(), sieve.primes().end(), before.begin(), before.end());
    BOOST_CHECK_EQUAL(sieve.last(), 97u);
}

BOOST_AUTO_TEST_CASE(test_sieve_beyond_matches_full_sieve) {
    constexpr std::uint64_t limit = 1'000'000u;
    const utility::Sieve<std::uint64_t> small(1'000u);
    const utility::Sieve<std::uint64_t> full(limit);
    std::optional<std::uint64_t> mismatch;
    for (std::uint64_t n = 1'000u; n < limit && !mismatch; ++n) {
        if (small.isPrime(n) != full.isPrime(n)) mismatch = n;
    }
    BOOST_TEST(!mismatch.has_value(), "Trial division disagrees with the full sieve at " << mismatch.value_or(0));
}

BOOST_AUTO_TEST_CASE(test_sieve_beyond_32_bit) {
    const utility::Sieve<std::uint64_t> sieve(65'536u);
    BOOST_CHECK(sieve.isPrime(4'294'967'291u));                        // Largest 32-bit prime
    BOOST_CHECK(!sieve.isPrime(4'294'967'295u));                       // 65535 * 65537
    BOOST_CHECK(!sieve.isPrime(4'292'870'399u));                       // 65519 * 65521
    BOOST_CHECK(!sieve.isPrime(4'294'836'225u));                       // 65535^2
    BOOST_CHECK_THROW(sieve.isPrime(4'294'967'296u), std::range_error); // 65536^2
}


BOOST_AUTO_TEST_CASE(test_sieve_minimum_size) {
    for (const unsigned int limit: {0u, 1u, 2u, 15u, 16u}) {
        const utility::Sieve<unsigned int> sieve(limit);
        BOOST_CHECK_EQUAL(sieve.size(), 6u);                      // 2, 3, 5, 7, 11, 13
        BOOST_CHECK_EQUAL(sieve.last(), 13u);
        BOOST_CHECK(!sieve.isPrime(255));                         // 3 * 5 * 17
        BOOST_CHECK_THROW(sieve.isPrime(256), std::range_error);  // 16^2
    }
}
BOOST_AUTO_TEST_SUITE_END()