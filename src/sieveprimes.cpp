// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2022 Glen S. Dayton. Rights reserved according to terms of included license.
#include <algorithm>
#include "sieveprimes.hpp"

[[maybe_unused]] auto __attribute__((visibility("default"))) utility::sievePrimes(const unsigned long upperLimit, std::vector<unsigned long>& primes) -> std::vector<unsigned long>& {
    const Sieve<unsigned long> sieve(upperLimit);

    // Sieve enforces a minimum size, so it may hold primes at or above the limit.
    const auto end = std::ranges::lower_bound(sieve.primes(), upperLimit);
    primes.assign(sieve.primes().begin(), end);
    return primes;
}


template class __attribute__((visibility("default"))) utility::Sieve<unsigned int>;
template class __attribute__((visibility("default"))) utility::Sieve<unsigned long>;
template class __attribute__((visibility("default"))) utility::Sieve<unsigned long long>;
