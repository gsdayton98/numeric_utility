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


// Every prime below 2^16, enough to factor any 32-bit number.
std::vector<unsigned int> utility::Factor::primes{utility::Sieve<unsigned int>{0x1'0000u}.primes()};

std::mutex utility::Factor::cacheLock{};
std::map<unsigned int, std::vector<utility::Factor> > utility::Factor::cache{};


namespace {
    auto unlockedPreloadCache(const unsigned int upperLimit) -> void
    {
        for (const auto prime: utility::Factor::primes) {
            if (prime >= upperLimit) break;
            auto primePower = prime;
            unsigned int exponent = 1;
            for (;;) {
                const std::vector factors = {utility::Factor{prime, exponent}};
                utility::Factor::cache[primePower] = factors;
                // Stop before the next power reaches the limit or overflows.
                if (primePower > (upperLimit - 1) / prime) break;
                primePower *= prime;
                ++exponent;
            }
        }
    }
}


auto utility::Factor::preloadCache(const unsigned int upperLimit) -> void
{
    std::unique_lock<std::mutex> lock{cacheLock};
    unlockedPreloadCache(upperLimit);
}


auto utility::Factor::factor(unsigned int n) -> std::vector<utility::Factor>
{
    std::vector<Factor> factors{};
    const auto n0 = n;


    {
        std::unique_lock<std::mutex> lock{cacheLock};
        if (cache.empty()) {
            unlockedPreloadCache(0xFFFFu);
        }
    }

    for (const auto prime: primes) {
        if (n <= 1) break;

        {
            std::unique_lock<std::mutex> lock{cacheLock};
            if (cache.contains(n)) {
                factors.insert(factors.end(), cache[n].begin(), cache[n].end());
                if (n != n0) cache[n0] = factors;
                return factors;
            }
        }

        if (prime > n / prime) break;  // No factor <= sqrt(n) remains, so n is prime.
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
