// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2025 Glen S. Dayton. Rights reserved according to terms of included license.

#include <boost/test/unit_test.hpp>
#include <cstdint>
#include <optional>

#include <numeric_utility/miller_rabin.hpp>
#include <numeric_utility/sieveprimes.hpp>

BOOST_AUTO_TEST_SUITE(TestMillerRabin)


BOOST_AUTO_TEST_CASE(test_miller_rabin) {
    constexpr unsigned int sample = 65537u;

    const auto result = utility::millerRabin(sample);
    BOOST_CHECK(result);
}


BOOST_AUTO_TEST_CASE(test_miller_rabin_matches_sieve) {
    constexpr unsigned long limit = 1'000'000ul;
    const utility::Sieve<unsigned long> sieve(limit);
    std::optional<unsigned long> mismatch;
    for (unsigned long n = 0; n < limit && !mismatch; ++n) {
        if (utility::millerRabin(n) != sieve.isPrime(n)) mismatch = n;
    }
    BOOST_TEST(!mismatch.has_value(), "millerRabin disagrees with the sieve at " << mismatch.value_or(0));
}


BOOST_AUTO_TEST_CASE(test_miller_rabin_large_primes) {
    for (const std::uint64_t prime: {
             2'147'483'647ul,                   // 2^31 - 1
             4'294'967'291ul,                   // Largest 32-bit prime
             4'294'967'311ul,                   // Smallest prime above 2^32
             999'999'999'989ul,
             2'305'843'009'213'693'951ul,       // 2^61 - 1
             18'446'744'073'709'551'557ul}) {   // Largest 64-bit prime
        BOOST_TEST(utility::millerRabin(prime), prime << " is prime");
    }
}


BOOST_AUTO_TEST_CASE(test_miller_rabin_large_composites) {
    for (const std::uint64_t composite: {
             561ul,                             // Carmichael number
             3'215'031'751ul,                   // Strong pseudoprime to bases 2, 3, 5, 7
             4'294'967'297ul,                   // 2^32 + 1 = 641 * 6700417
             2'305'843'009'213'693'953ul,       // 2^61 + 1
             3'825'123'056'546'413'051ul,       // Strong pseudoprime to bases 2 through 31
             18'446'743'979'220'271'189ul,      // 4294967291 * 4294967279
             18'446'744'030'759'878'681ul,      // 4294967291^2
             18'446'744'073'709'551'615ul}) {   // 2^64 - 1
        BOOST_TEST(!utility::millerRabin(composite), composite << " is composite");
    }
}


// Every prime factor of a base, where the base reduces to 0 mod n and must be skipped.
BOOST_AUTO_TEST_CASE(test_miller_rabin_base_factors) {
    for (const std::uint64_t prime: {3ul, 5ul, 13ul, 19ul, 73ul, 193ul, 407'521ul, 299'210'837ul}) {
        BOOST_TEST(utility::millerRabin(prime), prime << " is prime");
    }
    for (const std::uint64_t composite: {
             407'521ul * 407'521ul,
             299'210'837ul * 299'210'837ul,
             299'210'837ul * 407'521ul}) {
        BOOST_TEST(!utility::millerRabin(composite), composite << " is composite");
    }
}


BOOST_AUTO_TEST_CASE(test_miller_rabin_matches_sieve_above_2_32) {
    constexpr std::uint64_t first = 1ul << 32;
    constexpr std::uint64_t count = 200'000ul;
    const utility::Sieve<unsigned long> sieve(1ul << 17);   // isPrime is exact below 2^34
    std::optional<std::uint64_t> mismatch;
    for (auto n = first; n < first + count && !mismatch; ++n) {
        if (utility::millerRabin(n) != sieve.isPrime(n)) mismatch = n;
    }
    BOOST_TEST(!mismatch.has_value(), "millerRabin disagrees with the sieve at " << mismatch.value_or(0));
}
BOOST_AUTO_TEST_SUITE_END()
