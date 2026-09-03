// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2022 Glen S. Dayton. Rights reserved according to terms of included license.
#ifndef SIEVEPRIMES_HPP
#define SIEVEPRIMES_HPP
#include <stdexcept>
#include <vector>


namespace utility {
    [[maybe_unused]] auto __attribute__((visibility("default"))) sievePrimes(unsigned long upperLimit, std::vector<unsigned long> &primes) -> std::vector<unsigned long> &;

    template <typename Unsigned>
    class Sieve {
    public:
        using primeIterator = std::vector<Unsigned>::const_iterator;
        /**
         * Construct an Erothsenes sieve.
         * @param upperLimit Maximum number to sieve to.
         * @throws Exceptions from the underlying STL containers.
         */
        [[maybe_unused]] explicit Sieve(Unsigned upperLimit);

        /**
         * Returns whether a number is prime, that is, in the sieve.
         * @param number Number to test
         * @return True if the number is prime.
         * @throw May throw a range_error if the number exceeds the capacity of the sieve.
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
        [[maybe_unused]] auto primes() const -> std::vector<Unsigned>& { return m_primes; }

    private:
        std::vector<bool> m_sieve;
        mutable std::vector<Unsigned> m_primes;
    };


    template <typename Unsigned>
    Sieve<Unsigned>::Sieve(Unsigned upperLimit)
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
    auto Sieve<Unsigned>::isPrime(Unsigned number) const -> bool {
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
    auto Sieve<Unsigned>::operator[](int n) const -> Unsigned {
        return m_primes[n];
    }

}

#endif //SIEVEPRIMES_HPP
