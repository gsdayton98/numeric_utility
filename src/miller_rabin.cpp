// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2025 Glen S. Dayton. Rights reserved according to terms of included license.
#include <numeric_utility/miller_rabin.hpp>
#include <numeric_utility/pow.hpp>


// Deterministic for all 64-bit n. Jim Sinclair's seven bases have no common strong pseudoprime below 2^64
// (verified exhaustively; see https://miller-rabin.appspot.com). Unlike Bach's bound, this doesn't assume
// the Riemann hypothesis.
auto utility::millerRabin(const std::uint64_t n) -> bool {
    static constexpr std::uint64_t aBase[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    if (n == 2) return true;
    if ((n & 1) == 0 || n < 2) return false;

    const auto n1 = n - 1;
    auto d = n1;
    auto s = 0u;
    while ((d & 1) == 0)
    {
        ++s;
        d >>= 1;
    }

    // At this point n = pow(2, s)*d + 1
    for (const auto base: aBase)
    {
        // A base that is a multiple of n proves nothing, so skip it.
        const auto a = base % n;
        if (a == 0) continue;
        auto x = powmod(a, d, n);
        if (x == 1 || x == n1) continue;
        auto witness = true;
        for (auto r = 1u; r < s && witness; ++r)
        {
            x = powmod(x, std::uint64_t{2}, n);
            if (x == n1) witness = false;
        }
        if (witness) return false;
    }
    return true;
}
