// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2022 Glen S. Dayton. Rights reserved according to terms of included license.
#include <boost/test/unit_test.hpp>
#include <cstdint>
#include <stdexcept>
#include <vector>
#include "sieveprimes.hpp"
BOOST_AUTO_TEST_SUITE(TestSieve)
BOOST_AUTO_TEST_CASE(test_sieve) {
    using SieveType = utility::Sieve<unsigned int>;
    SieveType sieve(10);

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
    BOOST_CHECK_THROW(sieve.isPrime(101), std::runtime_error);
}


BOOST_AUTO_TEST_CASE(test_array_sieve) {
    using SieveType = utility::Sieve<unsigned int>;
    SieveType sieve(10);

    BOOST_CHECK_EQUAL(sieve[0], 2);
    BOOST_CHECK_EQUAL(sieve[1], 3);
    BOOST_CHECK_EQUAL(sieve[2], 5);
    BOOST_CHECK_EQUAL(sieve.size(), 4);
    BOOST_CHECK_EQUAL(sieve.last(), 7);
}

BOOST_AUTO_TEST_CASE(test_sieve_capacity) {
    // A sieve of size s knows the primes below s, so it decides n only when n < s^2.
    const utility::Sieve<unsigned int> sieve11(11);
    BOOST_CHECK(!sieve11.isPrime(119));                                 // 7 * 17
    BOOST_CHECK_THROW(sieve11.isPrime(121), std::range_error);          // Needs 11

    const utility::Sieve<unsigned int> sieve12(12);
    BOOST_CHECK(!sieve12.isPrime(121));                                 // 11^2
    BOOST_CHECK(sieve12.isPrime(127));
    BOOST_CHECK(!sieve12.isPrime(143));                                 // 11 * 13
    BOOST_CHECK_THROW(sieve12.isPrime(144), std::range_error);
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
    for (std::uint64_t n = 1'000u; n < limit; ++n) {
        if (small.isPrime(n) != full.isPrime(n)) {
            BOOST_REQUIRE_EQUAL(small.isPrime(n), full.isPrime(n));
        }
    }
}

BOOST_AUTO_TEST_CASE(test_sieve_beyond_32_bit) {
    const utility::Sieve<std::uint64_t> sieve(65'536u);
    BOOST_CHECK(sieve.isPrime(4'294'967'291u));                        // Largest 32-bit prime
    BOOST_CHECK(!sieve.isPrime(4'294'967'295u));                       // 65535 * 65537
    BOOST_CHECK(!sieve.isPrime(4'292'870'399u));                       // 65519 * 65521
    BOOST_CHECK(!sieve.isPrime(4'294'836'225u));                       // 65535^2
    BOOST_CHECK_THROW(sieve.isPrime(4'294'967'296u), std::range_error); // 65536^2
}
BOOST_AUTO_TEST_SUITE_END()