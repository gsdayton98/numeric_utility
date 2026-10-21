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
/// @file
/// @brief The sieve of Eratosthenes, as a function and as a class that also answers primality queries.


namespace utility {
    /**
     * The primes below a limit, found by the sieve of Eratosthenes, which can then test larger numbers.
     *
     * Construction takes O(n log log n) time for n = upperLimit and holds one bit per number plus one
     * Unsigned per prime. Instantiated in the library for unsigned int, unsigned long and unsigned long long.
     *
     * @code
     * utility::Sieve<unsigned int> sieve{100};
     * sieve.size();          // 25 primes below 100
     * sieve[3];              // 7, the fourth prime
     * sieve.isPrime(97);     // true
     * sieve.isPrime(9'991);  // false: 97 * 103, found by trial division with the sieve's primes
     * for (const auto p : sieve) std::cout << p << ' ';
     * @endcode
     * @tparam Unsigned The unsigned integer type of the primes.
     */
    template <typename Unsigned>
    class Sieve {
    public:
        /// The type of the iterators over the primes.
        using primeIterator = std::vector<Unsigned>::const_iterator;

        /**
         * Construct an Erothsenes sieve.
         * @param upperLimit Sieve the numbers below upperLimit.
         * @throws Exceptions from the underlying STL containers.
         */
        [[maybe_unused]] explicit Sieve(size_t upperLimit);

        /**
         * Returns whether a number is prime.
         *
         * Numbers below upperLimit are looked up in the sieve. Larger ones up to upperLimit^2 are trial divided by
         * the sieve's primes, and larger still go to millerRabin. It does not modify the sieve,
         * so concurrent calls are safe.
         * @param number Number to test
         * @return True if the number is prime; false for 0 and 1.
         */
        [[maybe_unused]] auto isPrime(Unsigned number) const -> bool;

        /// Iterator to the smallest prime.
        /// @return The iterator.
        [[maybe_unused]] auto cbegin() const -> primeIterator { return m_primes.cbegin();}
        /// Iterator to the smallest prime, so a Sieve works in a range-for.
        /// @return The iterator.
        [[maybe_unused]] auto begin() const -> primeIterator { return m_primes.begin();}

        /// Iterator one past the largest prime.
        /// @return The iterator.
        [[maybe_unused]] auto cend() const -> primeIterator { return m_primes.cend();}
        /// Iterator one past the largest prime, so a Sieve works in a range-for.
        /// @return The iterator.
        [[maybe_unused]] auto end() const -> primeIterator { return m_primes.end();}

        /**
         * The n-th prime, counting from 0, so sieve[0] is 2. Unchecked: n must be less than size().
         * @param n Index of the prime.
         * @return The prime.
         */
        auto operator[](int n) const -> Unsigned;

        /**
         * Return the number of primes found.
         * @return Number of primes.
         */
        [[maybe_unused]] auto size() const -> Unsigned { return static_cast<Unsigned>(m_primes.size()); }

        /**
         * Returns the last prime found. Undefined if there are none, that is, if upperLimit was at most 2.
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
        if (limit && number >= limit) {
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
