// -*- mode: c++ -*-
//
// Created by Glen Dayton, new account on 11/29/22.
//

#ifndef FACTOR_HPP
#define FACTOR_HPP
#include <map>
#include <vector>

struct Factor {
    static std::vector<unsigned int> primes;
    static std::map<unsigned int, std::vector<Factor> > cache;

    unsigned int prime;
    unsigned int exponent;

    static auto preloadCache(unsigned int upperLimit) -> void;

    static auto factor(unsigned int n) -> std::vector<Factor>;

    static auto evaluate(const std::vector<Factor>&) -> unsigned int;
};

// Help testing and debugging
[[maybe_unused]] auto operator<(const Factor& left, const Factor& right) -> bool;
[[maybe_unused]] auto operator==(const Factor& left, const Factor& right) -> bool;
[[maybe_unused]] auto operator<<(std::ostream&, const Factor&) -> std::ostream&;

#endif //FACTOR_HPP
