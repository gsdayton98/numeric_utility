// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2026 Glen S. Dayton. Rights reserved according to terms of included license.
#ifndef NUMERIC_UTILITY_TOTIENT_HPP
#define NUMERIC_UTILITY_TOTIENT_HPP
#include <cstddef>
#include <limits>
#include <stdexcept>
#include <vector>
#include "concepts.hpp"


namespace utility {
    /**
     * A table of Euler's totient φ(n), the count of 1 ≤ k ≤ n coprime to n, for every n up to a limit.
     *
     * Built with a sieve: φ(n) = n ∏(1 − 1/p) over the primes p dividing n, applied to every multiple of each
     * prime. That takes O(n log log n) time and one Unsigned per entry.
     * φ(0) is 0, since no k satisfies 1 ≤ k ≤ 0.
     */
    template <Unsigned Number>
    class Totient {
    public:
        /**
         * Compute φ(n) for 0 ≤ n ≤ maxNumber.
         * @param maxNumber The largest n in the table.
         * @throws std::length_error if maxNumber is the largest Number, so the table size doesn't fit.
         * @throws Exceptions from the underlying STL containers.
         */
        explicit Totient(Number maxNumber);

        /**
         * Euler's totient of n.
         * @throws std::range_error if n is beyond the table.
         */
        [[nodiscard]] auto phi(Number n) const -> Number;

        /// Euler's totient of n, unchecked: n must be at most maxNumber.
        [[nodiscard]] auto operator[](Number n) const -> Number { return m_phi[n]; }

        /// The number of entries, maxNumber + 1.
        [[nodiscard]] auto size() const -> std::size_t { return m_phi.size(); }

        /// The whole table, indexed by n.
        [[nodiscard]] auto values() const -> const std::vector<Number>& { return m_phi; }

    private:
        std::vector<Number> m_phi;
    };


    template <Unsigned Number>
    Totient<Number>::Totient(const Number maxNumber) {
        if (maxNumber == std::numeric_limits<Number>::max()) {
            throw std::length_error("Totient: the table size doesn't fit in the type");
        }
        m_phi.resize(static_cast<std::size_t>(maxNumber) + 1);
        for (std::size_t n = 0; n < m_phi.size(); ++n) m_phi[n] = static_cast<Number>(n);

        // Untouched entries above 1 are primes, since no smaller prime has divided them.
        for (Number prime = 2; prime <= maxNumber; ++prime) {
            if (m_phi[prime] != prime) continue;
            for (Number multiple = prime; ; multiple += prime) {
                m_phi[multiple] -= m_phi[multiple] / prime;
                if (multiple > maxNumber - prime) break; // The next multiple would pass the end, or overflow.
            }
        }
    }


    template <Unsigned Number>
    auto Totient<Number>::phi(const Number n) const -> Number {
        if (n >= m_phi.size()) throw std::range_error("Totient: n is beyond the table");
        return m_phi[n];
    }
}

#endif //NUMERIC_UTILITY_TOTIENT_HPP
