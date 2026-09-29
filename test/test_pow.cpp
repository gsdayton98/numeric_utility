// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2025 Glen S. Dayton. Rights reserved according to terms of included license.

#include <boost/test/unit_test.hpp>
#include <cstdint>
#include <limits>
#include <vector>

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


// Comparison with reference implementations.
//
// powmod is compared against square-and-multiply in unsigned __int128, which is exact for moduli up to 2^64 because a
// product of two residues then fits. pow wraps modulo 2^N, and unsigned __int128 wraps modulo 2^128, a multiple of it,
// so the same arithmetic truncated to N bits is the reference. U128 has no wider type, so it is compared with cpp_int.
namespace {
  using boost::multiprecision::cpp_int;

  auto refPowmod(U128 base, U128 exponent, const U128 modulus) -> U128 {
    if (modulus < 2) return 0;
    U128 result = 1;
    base %= modulus;
    for (; exponent > 0; exponent >>= 1) {
      if (exponent & 1) result = result * base % modulus;
      base = base * base % modulus;
    }
    return result;
  }

  auto refPowWrap(U128 base, U128 exponent) -> U128 {
    U128 result = 1;
    for (; exponent > 0; exponent >>= 1) {
      if (exponent & 1) result *= base;
      base *= base;
    }
    return result;
  }

  auto toBig(const U128 value) -> cpp_int {
    return (cpp_int{static_cast<U64>(value >> 64)} << 64) | cpp_int{static_cast<U64>(value)};
  }

  /// Values where overflow starts: zero and one, the ends of the range, the halfway points, and the values around
  /// 2^(N/2), the largest whose square fits in N bits.
  template <typename T>
  auto boundaryValues() -> std::vector<T> {
    constexpr unsigned bits = std::numeric_limits<T>::digits;
    constexpr T max = std::numeric_limits<T>::max();
    const T root = static_cast<T>(T{1} << (bits / 2));
    return {0, 1, 2, 3, 4, 5, 7, 10,
            static_cast<T>(root - 1), root, static_cast<T>(root + 1), static_cast<T>(root + 2),
            static_cast<T>(max / 2), static_cast<T>(max / 2 + 1), static_cast<T>(max / 3),
            static_cast<T>(max - 3), static_cast<T>(max - 2), static_cast<T>(max - 1), max};
  }

  template <typename T>
  void checkPowmodAgainstReference() {
    const auto values = boundaryValues<T>();
    for (const T modulus: values) {
      for (const T base: values) {
        for (const T exponent: values) {
          const auto expected = static_cast<T>(refPowmod(base, exponent, modulus));
          BOOST_CHECK_MESSAGE(utility::powmod(base, exponent, modulus) == expected,
                              "powmod(" << +base << ", " << +exponent << ", " << +modulus << ") is "
                              << +utility::powmod(base, exponent, modulus) << ", expected " << +expected);
        }
      }
    }
  }

  template <typename T>
  void checkPowAgainstReference() {
    const auto values = boundaryValues<T>();
    for (const T base: values) {
      for (const T exponent: values) {
        const auto expected = static_cast<T>(refPowWrap(base, exponent));
        BOOST_CHECK_MESSAGE(utility::pow(base, exponent) == expected,
                            "pow(" << +base << ", " << +exponent << ") is " << +utility::pow(base, exponent)
                            << ", expected " << +expected);
      }
    }
  }
}

BOOST_AUTO_TEST_CASE(test_powmod_matches_reference_at_boundaries) {
  checkPowmodAgainstReference<U8>();
  checkPowmodAgainstReference<U16>();
  checkPowmodAgainstReference<U32>();
  checkPowmodAgainstReference<U64>();
}

// Every 8-bit modulus and base, with exponents up to 40 and at the top of the range.
BOOST_AUTO_TEST_CASE(test_powmod_exhaustive_8) {
  std::vector<unsigned> exponents{254, 255};
  for (unsigned e = 0; e <= 40; ++e) exponents.push_back(e);
  for (unsigned modulus = 0; modulus < 256; ++modulus) {
    for (unsigned base = 0; base < 256; ++base) {
      for (const unsigned exponent: exponents) {
        const auto expected = static_cast<U8>(refPowmod(base, exponent, modulus));
        if (utility::powmod(static_cast<U8>(base), static_cast<U8>(exponent), static_cast<U8>(modulus)) != expected) {
          BOOST_ERROR("powmod(" << base << ", " << exponent << ", " << modulus << ") is wrong");
          return;
        }
      }
    }
  }
}

// Every 16-bit base against the eight largest moduli, whose squares overflow 32 bits.
BOOST_AUTO_TEST_CASE(test_powmod_top_moduli_16) {
  for (unsigned modulus = 65528; modulus < 65536; ++modulus) {
    for (unsigned base = 0; base < 65536; ++base) {
      for (const unsigned exponent: {2u, 3u, 40'000u, 65'535u}) {
        const auto expected = static_cast<U16>(refPowmod(base, exponent, modulus));
        if (utility::powmod(static_cast<U16>(base), static_cast<U16>(exponent), static_cast<U16>(modulus)) != expected) {
          BOOST_ERROR("powmod(" << base << ", " << exponent << ", " << modulus << ") is wrong");
          return;
        }
      }
    }
  }
}

BOOST_AUTO_TEST_CASE(test_powmod_matches_multiprecision_at_boundaries_128) {
  const auto values = boundaryValues<U128>();
  for (const U128 modulus: values) {
    for (const U128 base: values) {
      for (const U128 exponent: values) {
        const cpp_int expected = utility::powmod(toBig(base), toBig(exponent), toBig(modulus));
        const cpp_int actual = toBig(utility::powmod(base, exponent, modulus));
        BOOST_CHECK_MESSAGE(actual == expected,
                            "powmod(" << toBig(base) << ", " << toBig(exponent) << ", " << toBig(modulus) << ") is "
                            << actual << ", expected " << expected);
      }
    }
  }
}

BOOST_AUTO_TEST_CASE(test_pow_matches_reference_at_boundaries) {
  checkPowAgainstReference<U8>();
  checkPowAgainstReference<U16>();
  checkPowAgainstReference<U32>();
  checkPowAgainstReference<U64>();
}

BOOST_AUTO_TEST_CASE(test_pow_matches_multiprecision_at_boundaries_128) {
  const cpp_int modulus = cpp_int{1} << 128;
  const auto values = boundaryValues<U128>();
  for (const U128 base: values) {
    for (const U128 exponent: values) {
      const cpp_int expected = boost::multiprecision::powm(toBig(base), toBig(exponent), modulus);
      const cpp_int actual = toBig(utility::pow(base, exponent));
      BOOST_CHECK_MESSAGE(actual == expected,
                          "pow(" << toBig(base) << ", " << toBig(exponent) << ") is " << actual << ", expected " << expected);
    }
  }
}

// The cases that follow from the definition, whatever the width.
template <typename T>
void checkPowIdentities() {
  constexpr unsigned bits = std::numeric_limits<T>::digits;
  constexpr T max = std::numeric_limits<T>::max();
  BOOST_CHECK(utility::pow(T{0}, T{0}) == 1);           // Empty product.
  BOOST_CHECK(utility::pow(T{0}, T{1}) == 0);
  BOOST_CHECK(utility::pow(T{0}, T{5}) == 0);
  BOOST_CHECK(utility::pow(T{0}, max) == 0);
  BOOST_CHECK(utility::pow(T{1}, max) == 1);
  BOOST_CHECK(utility::pow(max, T{2}) == 1);            // (-1)^2 modulo 2^N
  BOOST_CHECK(utility::pow(max, T{3}) == max);          // (-1)^3
  BOOST_CHECK(utility::pow(T{2}, T{bits - 1}) == static_cast<T>(T{1} << (bits - 1)));
  BOOST_CHECK(utility::pow(T{2}, T{bits}) == 0);        // 2^N wraps to zero.
  BOOST_CHECK(utility::pow(T{2}, max) == 0);
}

BOOST_AUTO_TEST_CASE(test_pow_identities) {
  checkPowIdentities<U8>();
  checkPowIdentities<U16>();
  checkPowIdentities<U32>();
  checkPowIdentities<U64>();
  checkPowIdentities<U128>();
}

BOOST_AUTO_TEST_SUITE_END()