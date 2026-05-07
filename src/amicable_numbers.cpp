// -*- mode: c++ -*- ;
// ©2022 Glen S. Dayton. Project Euler has confidentiality rules, so do not copy nor publish this code.
#include "amicable_numbers.hpp"
#include <numeric>
#include "factor.hpp"

using namespace utility;

auto AmicableNumbers::d(const Number n) -> Number {
    auto divs = divisors(n, Factor::factor(n));
    return std::accumulate(divs.begin(), divs.end(), 0U);
}


auto AmicableNumbers::divisors(const Number n, const std::vector<Factor> &factors) -> std::vector<Number> {
    std::vector<Number> divisorsResults;

    std::vector divisorFactors{factors};
    for (auto &[prime, exponent]: divisorFactors) exponent = 0;

    Number divisor = 1U;
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