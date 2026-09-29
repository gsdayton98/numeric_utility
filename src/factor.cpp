// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2022 Glen S. Dayton. Rights reserved according to terms of included license.
#include <iostream>
#include "factor.hpp"
#include "pow.hpp"
#include "sieveprimes.hpp"

auto operator<(const utility::Factor &left, const utility::Factor &right) -> bool
{
    return left.prime < right.prime || (left.prime == right.prime && left.exponent < right.exponent);
}


auto operator==(const utility::Factor &left, const utility::Factor &right) -> bool
{
    return left.prime == right.prime && left.exponent == right.exponent;
}


auto operator<<(std::ostream &output, const utility::Factor &factor) -> std::ostream &
{
    return output << factor.prime << "^" << factor.exponent;
}


namespace {
    // Every prime below 2^16, enough to factor any 32-bit number. Built on first use rather than when the
    // library loads, which also makes it safe to use during static initialization elsewhere.
    auto primes() -> const std::vector<unsigned int>&
    {
        static const std::vector<unsigned int> table{utility::Sieve<unsigned int>{0x1'0000u}.primes()};
        return table;
    }
}


auto utility::Factor::preloadCache(unsigned int) -> void
{
}


auto utility::Factor::factor(unsigned int n) -> std::vector<utility::Factor>
{
    std::vector<Factor> factors{};

    for (const auto prime: primes()) {
        if (prime > n / prime) break;  // No factor <= sqrt(n) remains, so n is 1 or prime.
        unsigned int power = 0;
        while (n % prime == 0) {
            ++power;
            n /= prime;
        }
        if (power > 0) {
            factors.push_back(Factor{prime, power});
        }
    }
    if (n > 1) {
        factors.push_back(Factor(n, 1));
    }
    return factors;
}


auto utility::Factor::evaluate(const std::vector<utility::Factor> &factors) -> unsigned int
{
    unsigned int number = 1U;
    for (const auto [prime, exponent]: factors) {
        number *= pow(prime, exponent);
    }
    return number;
}
