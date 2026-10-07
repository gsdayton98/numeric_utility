// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2025 Glen S. Dayton. Rights reserved according to terms of included license.
    #include <cstddef>

#include <numeric_utility/miller_rabin.hpp>


namespace {
    // (a * b) % n. Below 2^32 the product fits in 64 bits, which avoids the slow 128-bit remainder.
    template<bool Wide>
    auto mulmod(const std::uint64_t a, const std::uint64_t b, const std::uint64_t n) -> std::uint64_t {
        if constexpr (Wide) return static_cast<std::uint64_t>(static_cast<unsigned __int128>(a) * b % n);
        else return a * b % n;
    }

    template<bool Wide>
    auto powmodN(std::uint64_t base, std::uint64_t exponent, const std::uint64_t n) -> std::uint64_t {
        std::uint64_t result = 1;
        while (exponent != 0) {
            if (exponent & 1) result = mulmod<Wide>(result, base, n);
            base = mulmod<Wide>(base, base, n);
            exponent >>= 1;
        }
        return result;
    }

    // Strong probable-prime test of odd n > 2 to each base; n - 1 = 2^s * d with d odd.
    template<bool Wide, std::size_t N>
    auto strongProbablePrime(const std::uint64_t n, const std::uint64_t (&bases)[N]) -> bool {
        const auto n1 = n - 1;
        auto d = n1;
        auto s = 0u;
        while ((d & 1) == 0) {
            ++s;
            d >>= 1;
        }

        // At this point n = pow(2, s)*d + 1
        for (const auto base: bases) {
            // A base that is a multiple of n proves nothing, so skip it.
            const auto a = base % n;
            if (a == 0) continue;
            auto x = powmodN<Wide>(a, d, n);
            if (x == 1 || x == n1) continue;
            auto witness = true;
            for (auto r = 1u; r < s && witness; ++r) {
                x = mulmod<Wide>(x, x, n);
                if (x == n1) witness = false;
            }
            if (witness) return false;
        }
        return true;
    }
}


// Deterministic for all 64-bit n.
// - Below 4,759,123,141, bases 2, 7 and 61 have no common strong pseudoprime (Jaeschke).
// - Above that, Jim Sinclair's seven bases have none below 2^64 (verified exhaustively; see
//   https://miller-rabin.appspot.com). Unlike Bach's bound, neither set assumes the Riemann hypothesis.
auto utility::millerRabin(const std::uint64_t n) -> bool {
    static constexpr std::uint64_t smallBases[] = {2, 7, 61};
    static constexpr std::uint64_t aBase[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    static constexpr std::uint64_t smallPrimes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    constexpr std::uint64_t smallLimit = 4'759'123'141ull;

    if (n < 2) return false;
    // Trial division rejects most composites before any modular exponentiation.
    for (const auto p: smallPrimes) {
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    if (n < (std::uint64_t{1} << 32)) return strongProbablePrime<false>(n, smallBases);
    if (n < smallLimit) return strongProbablePrime<true>(n, smallBases);
    return strongProbablePrime<true>(n, aBase);
}
