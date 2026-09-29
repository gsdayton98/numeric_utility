// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2022 Glen S. Dayton. Rights reserved according to terms of included license.
#include <numeric_utility/amicable_numbers.hpp>
#include <numeric>
#include <numeric_utility/factor.hpp>

using namespace utility;

auto AmicableNumbers::d(const unsigned int n) -> std::uint64_t {
    auto divs = divisors(n, Factor::factor(n));
    return std::accumulate(divs.begin(), divs.end(), std::uint64_t{0});
}


auto AmicableNumbers::divisors(const unsigned int n, const std::vector<Factor> &factors) -> std::vector<unsigned int> {
    std::vector<unsigned int> divisorsResults;

    std::vector divisorFactors{factors};
    for (auto &[prime, exponent]: divisorFactors) exponent = 0;

    unsigned int divisor = 1U;
    while (divisor < n) {
        divisor = Factor::evaluate(divisorFactors);
        if (divisor < n) {
            divisorsResults.push_back(divisor);

            // Step through the factors and divisor factors
            for (auto f = 0; f < divisorFactors.size(); ++f) {
                divisorFactors[f].exponent += 1;
                if (divisorFactors[f].exponent <= factors[f].exponent) {
                    break;
                }
                divisorFactors[f].exponent = 0;
            } // end for
        }
    }

    return divisorsResults;
}