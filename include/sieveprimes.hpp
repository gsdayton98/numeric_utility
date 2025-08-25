// -*- mode: c++ ;
//
// Created by Glen Dayton, new account on 11/13/22.
//

#ifndef EULER3_LARGEST_PRIME_FACTOR_SIEVEPRIMES_HPP
#define EULER3_LARGEST_PRIME_FACTOR_SIEVEPRIMES_HPP
#include <vector>


namespace utility {
    [[maybe_unused]] auto sievePrimes(unsigned long upperLimit, std::vector<unsigned long> &primes) -> std::vector<unsigned long> &;

    template <typename Unsigned>
    class Sieve {
    public:
        using primeIterator = typename std::vector<Unsigned>::const_iterator;

        [[maybe_unused]] explicit Sieve(Unsigned upperLimit);

        [[maybe_unused]] auto isPrime(Unsigned number) const -> bool;

        [[maybe_unused]] auto cbegin() const -> primeIterator { return m_primes.cbegin();}
        [[maybe_unused]] auto begin() const -> primeIterator { return m_primes.begin();}

        [[maybe_unused]] auto cend() const -> primeIterator { return m_primes.cend();}
        [[maybe_unused]] auto end() const -> primeIterator { return m_primes.end();}

        auto operator[](int n) const -> Unsigned;

        [[maybe_unused]] auto size() const -> Unsigned { return m_primes.size(); }

        [[maybe_unused]] auto last() const -> Unsigned { return m_primes.back(); }

        [[maybe_unused]] auto primes() const -> std::vector<Unsigned>& { return m_primes; }

    private:
        std::vector<bool> m_sieve;
        mutable std::vector<Unsigned> m_primes;
    };
}

#endif //EULER3_LARGEST_PRIME_FACTOR_SIEVEPRIMES_HPP
