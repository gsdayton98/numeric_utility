// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2025 Glen S. Dayton. Rights reserved according to terms of included license.

#include <boost/test/unit_test.hpp>

#include "pow.hpp"
#include "pow_multiprecision.hpp"

struct Sample {
  unsigned int base;
  unsigned int exponent;
  unsigned int expected;

  Sample(const unsigned int a, const unsigned int b, const unsigned int result) :
    base(a), exponent(b), expected(result)
  {}
};

BOOST_AUTO_TEST_SUITE(TestPow)

BOOST_AUTO_TEST_CASE(test_pow) {
  Sample samples[] = {
    Sample(42u, 0u, 1u),
    Sample(42u, 1u, 42u),
    Sample(42u, 2u, 42u*42u),
    Sample(42u, 9u, 42u*42u*42u*42u*42u*42u*42u*42u*42u)
  };
  for (const auto sample : samples)
  {
    BOOST_CHECK_EQUAL(utility::pow(sample.base, sample.exponent), sample.expected);
  }
}



struct ModSample {
  unsigned int base;
  unsigned int exponent;
  unsigned int modulus;
  unsigned int expected;

  ModSample(const unsigned int a, const unsigned int b, const unsigned int m, const unsigned int result) :
    base{a}, exponent{b}, modulus{m}, expected{result}
  {}
};

BOOST_AUTO_TEST_CASE(test_powmod) {
  ModSample samples[] = {
    ModSample(42u, 2u, 1u, 0u),
    ModSample(42u, 0u, 45u, 1u),
    ModSample(42u, 1u, 45u, 42u),
    ModSample(42u, 2u, 45u, 9u),
    ModSample(42u, 9u, 45u, 27u),
    ModSample(2u, 3u, 3u, 2u),
    ModSample(2u, 3u, 5u, 3u),
    ModSample(3u, 5u, 7u, 5u),
    ModSample(5u, 10u, 7u, 2u)
  };
  for (const auto sample : samples)
  {
    BOOST_CHECK_EQUAL(utility::powmod(sample.base, sample.exponent, sample.modulus), sample.expected);
  }
}

// Moduli whose square does not fit in the type, so base*base must not overflow.
BOOST_AUTO_TEST_CASE(test_powmod_large_modulus_8) {
  using U8 = unsigned char;
  constexpr U8 prime = 251u;  // Largest 8-bit prime
  BOOST_CHECK_EQUAL(utility::powmod(U8{2u}, U8{prime - 1u}, prime), U8{1u});                 // Fermat
  BOOST_CHECK_EQUAL(utility::powmod(U8{prime - 1u}, U8{2u}, prime), U8{1u});                 // (-1)^2
  BOOST_CHECK_EQUAL(utility::powmod(U8{prime - 1u}, U8{prime - 2u}, prime), U8{prime - 1u}); // (-1)^odd
  BOOST_CHECK_EQUAL(utility::powmod(U8{255u}, U8{2u}, prime), U8{16u});                      // 255 = 4 (mod p)
}

BOOST_AUTO_TEST_CASE(test_powmod_large_modulus_16) {
  using U16 = unsigned short;
  constexpr U16 prime = 65521u;  // Largest 16-bit prime
  BOOST_CHECK_EQUAL(utility::powmod(U16{2u}, U16{prime - 1u}, prime), U16{1u});                  // Fermat
  BOOST_CHECK_EQUAL(utility::powmod(U16{65534u}, U16{2u}, prime), U16{169u});                    // 65534 = 13 (mod p)
  BOOST_CHECK_EQUAL(utility::powmod(U16{prime - 1u}, U16{prime - 2u}, prime), U16{prime - 1u});  // (-1)^odd
}

BOOST_AUTO_TEST_CASE(test_powmod_large_modulus_32) {
  constexpr unsigned int prime = 4'294'967'291u;  // Largest 32-bit prime
  BOOST_CHECK_EQUAL(utility::powmod(3u, 1'000'000u, prime), 3'445'042'560u);
  BOOST_CHECK_EQUAL(utility::powmod(65'537u, 2u, prime), 131'078u);        // 65537^2 - p
  BOOST_CHECK_EQUAL(utility::powmod(2u, prime - 1u, prime), 1u);           // Fermat
  BOOST_CHECK_EQUAL(utility::powmod(123'456'789u, prime - 1u, prime), 1u); // Fermat
  BOOST_CHECK_EQUAL(utility::powmod(prime - 1u, 3u, prime), prime - 1u);   // (-1)^3
  BOOST_CHECK_EQUAL(utility::powmod(prime - 1u, prime - 2u, prime), prime - 1u); // (-1)^odd
}

BOOST_AUTO_TEST_CASE(test_powmod_large_modulus_64) {
  constexpr unsigned long prime = 18'446'744'073'709'551'557ul;  // Largest 64-bit prime
  constexpr unsigned long mersenne61 = (1ul << 61) - 1u;

  BOOST_CHECK_EQUAL(utility::powmod(3ul, 1'000'000ul, prime), 16'059'052'939'423'793'818ul);
  BOOST_CHECK_EQUAL(utility::powmod(2ul, prime - 1u, prime), 1ul);           // Fermat
  BOOST_CHECK_EQUAL(utility::powmod(4'294'967'311ul, prime - 1u, prime), 1ul);
  BOOST_CHECK_EQUAL(utility::powmod(prime - 1u, 2ul, prime), 1ul);           // (-1)^2
  BOOST_CHECK_EQUAL(utility::powmod(prime - 1u, prime - 2u, prime), prime - 1u); // (-1)^odd
  BOOST_CHECK_EQUAL(utility::powmod(2ul, 64ul, mersenne61), 8ul);            // 2^61 = 1 (mod 2^61 - 1)
  BOOST_CHECK_EQUAL(utility::powmod(2ul, mersenne61 - 1u, mersenne61), 1ul); // Fermat
}

BOOST_AUTO_TEST_CASE(test_powmod_large_modulus_64_long_long) {
  constexpr unsigned long long prime = 18'446'744'073'709'551'557ull;  // Largest 64-bit prime
  BOOST_CHECK_EQUAL(utility::powmod(3ull, 1'000'000ull, prime), 16'059'052'939'423'793'818ull);
  BOOST_CHECK_EQUAL(utility::powmod(2ull, prime - 1u, prime), 1ull);                // Fermat
  BOOST_CHECK_EQUAL(utility::powmod(prime - 1u, prime - 2u, prime), prime - 1u);    // (-1)^odd
}

BOOST_AUTO_TEST_CASE(test_powmod_large_modulus_128) {
  using U128 = unsigned __int128;
  const U128 prime = (U128{1} << 127) - 1u;  // Mersenne prime 2^127 - 1
  BOOST_CHECK(utility::powmod(U128{3}, prime - 1u, prime) == U128{1});      // Fermat
  BOOST_CHECK(utility::powmod(U128{2}, U128{128}, prime) == U128{2});       // 2^127 = 1 (mod p)
  BOOST_CHECK(utility::powmod(prime - 1u, U128{3}, prime) == prime - 1u);   // (-1)^3
}

BOOST_AUTO_TEST_CASE(test_powmod_multiprecision) {
  using boost::multiprecision::cpp_int;
  const cpp_int prime = (cpp_int{1} << 521) - 1;  // Mersenne prime 2^521 - 1
  BOOST_CHECK_EQUAL(utility::powmod(cpp_int{3}, prime - 1, prime), cpp_int{1});      // Fermat
  BOOST_CHECK_EQUAL(utility::powmod(cpp_int{2}, cpp_int{522}, prime), cpp_int{2});   // 2^521 = 1 (mod p)
  BOOST_CHECK_EQUAL(utility::powmod(prime - 1, cpp_int{3}, prime), prime - 1);       // (-1)^3
  BOOST_CHECK_EQUAL(utility::powmod(cpp_int{3}, cpp_int{1'000'000}, cpp_int{18'446'744'073'709'551'557ull}),
                    cpp_int{16'059'052'939'423'793'818ull});
  BOOST_CHECK_EQUAL(utility::powmod(cpp_int{42}, cpp_int{2}, cpp_int{1}), cpp_int{0});
}
BOOST_AUTO_TEST_SUITE_END()