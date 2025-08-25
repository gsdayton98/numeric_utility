// -*- mode: c++ ;
//
// Created by Glen Dayton, new account on 11/13/22.
//
#include "sieveprimes.hpp"
#include <stdexcept>

[[maybe_unused]] auto utility::sievePrimes(const unsigned long upperLimit, std::vector<unsigned long>& primes) -> std::vector<unsigned long>& {
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



template <typename Unsigned>
[[maybe_unused]] utility::Sieve<Unsigned>::Sieve(Unsigned upperLimit)
    : m_sieve(upperLimit,true),
      m_primes()
{
    m_sieve[0] = false;
    m_sieve[1] = false;
    for (Unsigned number = 4; number < m_sieve.size(); number += 2) m_sieve[number] = false;
    m_primes.push_back(2);

    for (Unsigned number = 3; number < m_sieve.size(); number += 2) {
        if (m_sieve[number]) {
            m_primes.push_back(number);
            for (auto markOff = 2*number; markOff < m_sieve.size(); markOff += number) {
                m_sieve[markOff] = false;
            }
        }
    }
}


template <typename Unsigned>
[[maybe_unused]] auto utility::Sieve<Unsigned>::isPrime(Unsigned number) const -> bool {
    if (number < m_sieve.size()) return m_sieve[number];
    if (number > m_sieve.size()*m_sieve.size()) throw std::range_error("Sieve isn't big enough");

    for (auto divisor: m_primes) {
        if (number % divisor == 0) return false;
        if (2*divisor > number) break;
    }
    m_primes.push_back(number);
    return true;
}

template <typename Unsigned>
auto utility::Sieve<Unsigned>::operator[](int n) const -> Unsigned {
    return m_primes[n];
}


template class utility::Sieve<unsigned int>;
template class utility::Sieve<unsigned long>;
template class utility::Sieve<unsigned long long>;
