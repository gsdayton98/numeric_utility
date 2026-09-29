// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2022 Glen S. Dayton. Rights reserved according to terms of included license.
#ifndef NUMERIC_UTILITY_SIEVEPRIMES_HPP
#define NUMERIC_UTILITY_SIEVEPRIMES_HPP
#include <algorithm>
#include <stdexcept>
#include <vector>
#include "isqrt.hpp"
#include "numeric_utility_export.h"


namespace utility {
    /**
     * Replace the contents of primes with the primes below upperLimit.
     * @return primes
     */
    [[maybe_unused]] NUMERIC_UTILITY_API auto sievePrimes(unsigned long upperLimit, std::vector<unsigned long> &primes) -> std::vector<unsigned long> &;

    template <typename Unsigned>
    class Sieve {
    public:
        /**
         * Minimum practical Sieve upperLimit.
         */
        static constexpr Unsigned MINIMUM_SIEVE_SIZE = 16;

        using primeIterator = std::vector<Unsigned>::const_iterator;
        /**
         * Construct an Erothsenes sieve.
         * @param upperLimit Sieve the numbers below upperLimit. the sieve covers numbers below max(upperLimit, MINIMUM_SIEVE_SIZE)"
         * @throws Exceptions from the underlying STL containers.
         */
        [[maybe_unused]] explicit Sieve(Unsigned upperLimit);

        /**
         * Returns whether a number is prime, that is, in the sieve.
         * @param number Number to test
         * @return True if the number is prime.
         * Numbers beyond the sieve are trial divided by its primes. It does not modify the sieve,
         * so concurrent calls are safe.
         * @throw range_error if the number is at least upperLimit^2, beyond the capacity of the sieve.
         */
        [[maybe_unused]] auto isPrime(Unsigned number) const -> bool;

        /**
         * Iterators into the container of prime numbers.
         */
        [[maybe_unused]] auto cbegin() const -> primeIterator { return m_primes.cbegin();}
        [[maybe_unused]] auto begin() const -> primeIterator { return m_primes.begin();}

        [[maybe_unused]] auto cend() const -> primeIterator { return m_primes.cend();}
        [[maybe_unused]] auto end() const -> primeIterator { return m_primes.end();}

        auto operator[](int n) const -> Unsigned;

        /**
         * Return the number of primes found.
         * @return Number of primes.
         */
        [[maybe_unused]] auto size() const -> Unsigned { return m_primes.size(); }

        /**
         * Returns the last prime found.
         * @return Last prime found.
         */
        [[maybe_unused]] auto last() const -> Unsigned { return m_primes.back(); }

        /**
         * Returns the vector of primes found.
         * @return Vector of primes.
         */
        [[maybe_unused]] auto primes() const -> const std::vector<Unsigned>& { return m_primes; }

    private:
        std::vector<bool> m_sieve;
        std::vector<Unsigned> m_primes;
    };


    template <typename Unsigned>
    Sieve<Unsigned>::Sieve(Unsigned upperLimit)
        : m_sieve(std::max(upperLimit, Sieve::MINIMUM_SIEVE_SIZE),true),
          m_primes()
    {
        // 0 and 1 are not primes.
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
    auto Sieve<Unsigned>::isPrime(Unsigned number) const -> bool {
        if (number < m_sieve.size()) return m_sieve[number];
        // Deciding the number needs every prime up to its square root, and the sieve only knows those below its size.
        if (isqrt(number) >= m_sieve.size()) throw std::range_error("Sieve isn't big enough");

        for (const auto divisor: m_primes) {
            if (divisor > number / divisor) break;
            if (number % divisor == 0) return false;
        }
        return true;
    }

    template <typename Unsigned>
    auto Sieve<Unsigned>::operator[](int n) const -> Unsigned {
        return m_primes[n];
    }

}

#endif //NUMERIC_UTILITY_SIEVEPRIMES_HPP
