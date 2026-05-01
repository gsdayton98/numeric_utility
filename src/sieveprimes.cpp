// -*- mode: c++;
//
// Created by Glen Dayton, new account on 11/13/22.
//
#include "sieveprimes.hpp"

[[maybe_unused]] auto __attribute__((visibility("default"))) utility::sievePrimes(const unsigned long upperLimit, std::vector<unsigned long>& primes) -> std::vector<unsigned long>& {
    std::vector sieve(upperLimit, true);

    primes.clear();
    primes.push_back(2u);
    for (unsigned int multiple = 2*2; multiple < upperLimit; multiple += 2) {
        sieve[multiple] = false;
    }

    for (unsigned int candidate=3u; candidate < upperLimit; candidate += 2u) {
        if (sieve[candidate] ) {
            primes.push_back(candidate);
            for (unsigned int multiple = 2*candidate; multiple < upperLimit; multiple += candidate) {
                sieve[multiple] = false;
            }
        }
    }

    return primes;
}


template class __attribute__((visibility("default"))) utility::Sieve<unsigned int>;
template class __attribute__((visibility("default"))) utility::Sieve<unsigned long>;
template class __attribute__((visibility("default"))) utility::Sieve<unsigned long long>;
