// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2025 Glen S. Dayton. Rights reserved according to terms of included license.
#include "miller_rabin.hpp"
#include <algorithm>
#include <cmath>
#include "pow.hpp"


// Deterministic for all 64-bit n: the bases 2 through 37 have no common strong pseudoprime below 3.3e24.
auto utility::millerRabin(const std::uint64_t n) -> bool {
    static std::uint64_t aBase[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
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
    std::uint64_t y = 0;
    const auto ln_n = std::log(n);
    const auto baseLimit = std::min(n - 2, static_cast<std::uint64_t>(std::floor(2.0*ln_n*ln_n)));
    for (const auto a: aBase)
    {
        if (a > baseLimit) break;
        auto x = powmod(a, d, n);
        if (x == 1 || x == n1) continue;
        for (auto trial = 1u; trial <= s; ++trial)
        {
            y = powmod(x, std::uint64_t{2}, n);
            if (y == 1 && x != 1 && x != n1) return false;
            x = y;
        }
        if (y != 1) return false;
    }
    return true;
}
