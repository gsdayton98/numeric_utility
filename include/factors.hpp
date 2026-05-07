//
// Created by Glen Dayton, new account on 6/11/24.
//

#ifndef FACTORS_HPP
#define FACTORS_HPP

#include <map>
namespace utility {
    using Number = unsigned int;

    ////
    // Factors
    // Represent a number as a set of its prime factors and their exponents.
    // For example 20 = 2×2×5 so its representation is (2,2)(5,1).
    // Factors performs multiplication by adding the exponents of matching primes, and adding entries for unmatched primes.
    // Factors divides by subtracting the exponents of matching primes, and adding negative exponent entries for
    // unmatched primes.
    //
    // Factors representation is useful for calculations using combinatorials without resorting to a large number
    // library.


    class __attribute__((visibility("default"))) Factors {
        using map_t = std::map<Number, int>;
        map_t factors;

        public:
        Factors() = default;

        Factors(std::initializer_list<map_t::value_type> a_factors);


        auto operator* (const Factors &right) const -> Factors;
        auto operator/ (const Factors &right) const -> Factors;

        auto print(std::ostream &) const -> std::ostream&;

        [[maybe_unused]] [[nodiscard]] auto asNumber() const -> Number;
    };

    auto operator<<(std::ostream &out, const Factors &factors) -> std::ostream&;
}
#endif //FACTORS_HPP
