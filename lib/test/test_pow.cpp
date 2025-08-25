#include <boost/test/unit_test.hpp>

#include <iostream>
#include "pow.hpp"

struct Sample {
  unsigned int base;
  unsigned int exponent;
  unsigned int expected;

  Sample(const unsigned int a, const unsigned int b, const unsigned int result) :
    base(a), exponent(b), expected(result)
  {}
};


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
    auto result = utility::powmod(sample.base, sample.exponent, sample.modulus);
    std::cout << "powmod(" << sample.base << "," << sample.exponent << "," << sample.modulus << ") "
    << result << " expecting " << sample.expected << "\n";
    BOOST_CHECK_EQUAL(utility::powmod(sample.base, sample.exponent, sample.modulus), sample.expected);
  }
}
