#include <boost/test/unit_test.hpp>
#include "sieveprimes.hpp"

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