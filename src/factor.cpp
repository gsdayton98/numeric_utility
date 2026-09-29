// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2022 Glen S. Dayton. Rights reserved according to terms of included license.
#include <iostream>
#include <map>
#include <mutex>
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
    // Built on first use rather than when the library loads, which also makes them safe to use
    // during static initialization elsewhere.

    // Every prime below 2^16, enough to factor any 32-bit number.
    auto primes() -> const std::vector<unsigned int>&
    {
        static const std::vector<unsigned int> table{utility::Sieve<unsigned int>{0x1'0000u}.primes()};
        return table;
    }

    // Guards cache(). std::mutex is constant-initialized, so it needs no such protection.
    std::mutex cacheLock;

    auto cache() -> std::map<unsigned int, std::vector<utility::Factor> >&
    {
        static std::map<unsigned int, std::vector<utility::Factor> > table;
        return table;
    }

    auto unlockedPreloadCache(const unsigned int upperLimit) -> void
    {
        for (const auto prime: primes()) {
            if (prime >= upperLimit) break;
            auto primePower = prime;
            unsigned int exponent = 1;
            for (;;) {
                const std::vector factors = {utility::Factor{prime, exponent}};
                cache()[primePower] = factors;
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
        if (cache().empty()) {
            unlockedPreloadCache(0xFFFFu);
        }
    }

    for (const auto prime: primes()) {
        if (n <= 1) break;

        {
            std::unique_lock<std::mutex> lock{cacheLock};
            if (auto& table = cache(); table.contains(n)) {
                factors.insert(factors.end(), table[n].begin(), table[n].end());
                if (n != n0) table[n0] = factors;
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
