// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2022 Glen S. Dayton. Rights reserved according to terms of included license.
#ifndef NUMERIC_UTILITY_SIEVEPRIMES_HPP
#define NUMERIC_UTILITY_SIEVEPRIMES_HPP
#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <vector>
#include "isqrt.hpp"
#include <numeric_utility/numeric_utility_export.h>

#include "miller_rabin.hpp"
#include "pow.hpp"


namespace utility {
    /**
     * @brief Perform a Eratosthenes Sieve to fill a vector with primes
     *
     * Creates a bit vector up to upperLimit long to sieve for primes. As primes are found pushes the prime to end of
     * the vector.
     *
     * @param upperLimit    Create a list of primes below upperLimit. UpperLimits of 0, 1 will yield no primes.
     * @param primes        A vector which will be cleared and filled with a list of primes below the upperLimit.
     *
     * @return primes       The input vector filled with the primes found. Will be empty if upperLimit is 0 or 1.
     * @throws              Exceptions from the underlying STL containers.
     */
    [[maybe_unused]] NUMERIC_UTILITY_API auto sievePrimes(unsigned long upperLimit, std::vector<unsigned long> &primes) -> std::vector<unsigned long> &;

    template <typename Unsigned>
    class Sieve {
    public:
        using primeIterator = std::vector<Unsigned>::const_iterator;

        /**
         * Construct an Erothsenes sieve.
         * @param upperLimit Sieve the numbers below upperLimit.
         * @throws Exceptions from the underlying STL containers.
         */
        [[maybe_unused]] explicit Sieve(size_t upperLimit);

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
        [[maybe_unused]] auto size() const -> Unsigned { return static_cast<Unsigned>(m_primes.size()); }

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
        std::vector<bool> m_sieve;      // Work area for the sieve
        std::vector<Unsigned> m_primes; // Primes actually found via the sieve.
    };


    template <typename Unsigned>
    Sieve<Unsigned>::Sieve(size_t upperLimit)
        : m_sieve(upperLimit,true),
          m_primes()
    {
        for (auto notPrime = 0ul; notPrime < std::min(upperLimit, 2ul); ++notPrime) {
            m_sieve[notPrime] = false;
        }

        for (auto p = 2ul; p < m_sieve.size(); ++p) {
            if (m_sieve[p]) {
                m_primes.push_back(static_cast<Unsigned>(p));
            }
            // Multiples below p*p have already been parked off.
            if (auto first = utility::checkedMul(p, p); first) {
                for (auto multiple= first.value(); multiple < m_sieve.size(); multiple+= p) {
                    m_sieve[multiple] = false;
                }
            }
        }
    }


    template <typename Unsigned>
    auto Sieve<Unsigned>::isPrime(Unsigned number) const -> bool {
        if (number < 2) {
            return false;
        }

        if (number < m_sieve.size()) return m_sieve[number];

        auto limit = utility::checkedMul(m_sieve.size(), m_sieve.size());
        if (limit && number > limit) {
            return utility::millerRabin(number);
        }

        for (const auto divisor: m_primes) {
            if (divisor > number / divisor) break;
            if (number % divisor == 0) return false;
        }
        return true;
    }

    template <typename Unsigned>
    auto Sieve<Unsigned>::operator[](int n) const -> Unsigned {
        return m_primes[static_cast<std::size_t>(n)];
    }

}

#endif //NUMERIC_UTILITY_SIEVEPRIMES_HPP
