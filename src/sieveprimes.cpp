// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2022 Glen S. Dayton. Rights reserved according to terms of included license.
#include <algorithm>
#include <numeric_utility/sieveprimes.hpp>

// These come before sievePrimes, which uses Sieve<unsigned long>. GCC ignores the export attribute on an
// explicit instantiation that follows an implicit one, leaving the symbols hidden.
template class NUMERIC_UTILITY_API utility::Sieve<unsigned int>;
template class NUMERIC_UTILITY_API utility::Sieve<unsigned long>;
template class NUMERIC_UTILITY_API utility::Sieve<unsigned long long>;

[[maybe_unused]] NUMERIC_UTILITY_API auto utility::sievePrimes(const unsigned long upperLimit, std::vector<unsigned long>& primes) -> std::vector<unsigned long>& {
    const Sieve<unsigned long> sieve(upperLimit);

    // Sieve enforces a minimum size, so it may hold primes at or above the limit.
    const auto end = std::ranges::lower_bound(sieve.primes(), upperLimit);
    primes.assign(sieve.primes().begin(), end);
    return primes;
}
