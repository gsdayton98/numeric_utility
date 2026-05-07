//
// Created by Glen Dayton, new account on 6/11/24.
//
#include <ostream>
#include "factors.hpp"

using namespace utility;

auto ipow(Number a, int b) -> Number {
    Number result = 1U;

    if (a > 0U) {
        while (b > 0U) {
            if (b & 1) result *= a;
            a *= a;
            b >>= 1;
        }
    }

    return result;
}


Factors::Factors(std::initializer_list<map_t::value_type> a_factors) : factors{a_factors}
{ }


auto Factors::operator* (const Factors &right) const -> Factors {
    Factors result = right;

    for (auto factor : factors) {
        if (result.factors.find(factor.first) != result.factors.end()) {
            result.factors[factor.first] += factor.second;
        } else {
            result.factors[factor.first] = factor.second;
        }
    }

    return std::move(result);
}


auto Factors::operator/ (const Factors &right) const -> Factors{
    Factors result = *this;

    for (auto factor : right.factors) {
        if (result.factors.find(factor.first) != result.factors.end()) {
            result.factors[factor.first] -= factor.second;
        } else {
            result.factors[factor.first] = -factor.second;
        }
        // If the factor reduces to 1, eliminate it
        if (result.factors[factor.first] == 0) {
            result.factors.erase(factor.first);
        }
    }

    return std::move(result);
}


auto Factors::print(std::ostream &out) const -> std::ostream& {
    out << "[";
    for (auto factor : factors) {
        out << " " << factor.first << "**" << factor.second;
    }
    out << " ]";
    return out;
}


auto operator<<(std::ostream &out, const Factors &factors) -> std::ostream &  {
    return factors.print(out);
}


auto  Factors::asNumber() const -> Number {
    Number numerator = 1U;

    for (auto factor : factors) {
        numerator *= ipow(factor.first, factor.second);
    }

    return numerator;
}
