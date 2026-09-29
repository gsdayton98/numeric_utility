// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2025 Glen S. Dayton. Rights reserved according to terms of included license.

#include <boost/test/unit_test.hpp>
#include <cstdint>

#include <numeric_utility/pow.hpp>
#include <numeric_utility/pow_multiprecision.hpp>

using U8 = std::uint8_t;
using U16 = std::uint16_t;
using U32 = std::uint32_t;
using U64 = std::uint64_t;
using U128 = unsigned __int128;

// Every type powmod is specialized for satisfies its Unsigned constraint.
static_assert(utility::Unsigned<U8> && utility::Unsigned<U16> && utility::Unsigned<U32>
              && utility::Unsigned<U64> && utility::Unsigned<U128>);

struct Sample {
  U32 base;
  U32 exponent;
  U32 expected;

  Sample(const U32 a, const U32 b, const U32 result) :
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
  U32 base;
  U32 exponent;
  U32 modulus;
  U32 expected;

  ModSample(const U32 a, const U32 b, const U32 m, const U32 result) :
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
  constexpr U8 prime = 251u;  // Largest 8-bit prime
  BOOST_CHECK_EQUAL(utility::powmod(U8{2u}, U8{prime - 1u}, prime), U8{1u});                 // Fermat
  BOOST_CHECK_EQUAL(utility::powmod(U8{prime - 1u}, U8{2u}, prime), U8{1u});                 // (-1)^2
  BOOST_CHECK_EQUAL(utility::powmod(U8{prime - 1u}, U8{prime - 2u}, prime), U8{prime - 1u}); // (-1)^odd
  BOOST_CHECK_EQUAL(utility::powmod(U8{255u}, U8{2u}, prime), U8{16u});                      // 255 = 4 (mod p)
}

BOOST_AUTO_TEST_CASE(test_powmod_large_modulus_16) {
  constexpr U16 prime = 65521u;  // Largest 16-bit prime
  BOOST_CHECK_EQUAL(utility::powmod(U16{2u}, U16{prime - 1u}, prime), U16{1u});                  // Fermat
  BOOST_CHECK_EQUAL(utility::powmod(U16{65534u}, U16{2u}, prime), U16{169u});                    // 65534 = 13 (mod p)
  BOOST_CHECK_EQUAL(utility::powmod(U16{prime - 1u}, U16{prime - 2u}, prime), U16{prime - 1u});  // (-1)^odd
}

BOOST_AUTO_TEST_CASE(test_powmod_large_modulus_32) {
  constexpr U32 prime = 4'294'967'291u;  // Largest 32-bit prime
  BOOST_CHECK_EQUAL(utility::powmod(U32{3}, U32{1'000'000}, prime), U32{3'445'042'560u});
  BOOST_CHECK_EQUAL(utility::powmod(U32{65'537}, U32{2}, prime), U32{131'078});        // 65537^2 - p
  BOOST_CHECK_EQUAL(utility::powmod(U32{2}, U32{prime - 1u}, prime), U32{1});           // Fermat
  BOOST_CHECK_EQUAL(utility::powmod(U32{123'456'789}, U32{prime - 1u}, prime), U32{1}); // Fermat
  BOOST_CHECK_EQUAL(utility::powmod(U32{prime - 1u}, U32{3}, prime), U32{prime - 1u});  // (-1)^3
  BOOST_CHECK_EQUAL(utility::powmod(U32{prime - 1u}, U32{prime - 2u}, prime), U32{prime - 1u}); // (-1)^odd
}

BOOST_AUTO_TEST_CASE(test_powmod_large_modulus_64) {
  constexpr U64 prime = 18'446'744'073'709'551'557u;  // Largest 64-bit prime
  constexpr U64 mersenne61 = (U64{1} << 61) - 1u;

  BOOST_CHECK_EQUAL(utility::powmod(U64{3}, U64{1'000'000}, prime), U64{16'059'052'939'423'793'818u});
  BOOST_CHECK_EQUAL(utility::powmod(U64{2}, U64{prime - 1u}, prime), U64{1});             // Fermat
  BOOST_CHECK_EQUAL(utility::powmod(U64{4'294'967'311u}, U64{prime - 1u}, prime), U64{1});
  BOOST_CHECK_EQUAL(utility::powmod(U64{prime - 1u}, U64{2}, prime), U64{1});             // (-1)^2
  BOOST_CHECK_EQUAL(utility::powmod(U64{prime - 1u}, U64{prime - 2u}, prime), U64{prime - 1u}); // (-1)^odd
  BOOST_CHECK_EQUAL(utility::powmod(U64{2}, U64{64}, mersenne61), U64{8});                // 2^61 = 1 (mod 2^61 - 1)
  BOOST_CHECK_EQUAL(utility::powmod(U64{2}, U64{mersenne61 - 1u}, mersenne61), U64{1});   // Fermat
}

BOOST_AUTO_TEST_CASE(test_powmod_large_modulus_128) {
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