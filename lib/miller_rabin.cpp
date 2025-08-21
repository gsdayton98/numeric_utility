//
// Created by Glen Dayton on 2/26/25.
//
#include "miller_rabin.hpp"
#include <cmath>
#include "pow.hpp"


// n < 4,759,123,141
auto millerRabin(const unsigned long n) -> bool {
    static unsigned long aBase[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
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
    auto y = 0u;
    const auto ln_n = std::log(n);
    const unsigned int baseLimit = std::min(n - 2, static_cast<unsigned long>(std::floor(2.0*ln_n*ln_n)));
    for (const auto a: aBase)
    {
        if (a > baseLimit) break;
        auto x = numutil::powmod(a, d, n);
        if (x == 1 || x == n1) continue;
        for (auto trial = 1u; trial <= s; ++trial)
        {
            y = numutil::powmod(x, 2ul, n);
            if ((y == 1) && (x != 1) && (x != n1)) return false;
            x = y;
        }
        if (y != 1) return false;
    }
    return true;
}
