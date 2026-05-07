// -*- mode: c++ -*-
//
// Created by Glen Dayton, new account on 11/29/22.
//

#ifndef FACTOR_HPP
#define FACTOR_HPP
#include <map>
#include <vector>
namespace utility {
    struct __attribute__((visibility("default")))  Factor {
        static std::vector<unsigned int> primes;
        static std::map<unsigned int, std::vector<Factor> > cache;

        unsigned int prime;
        unsigned int exponent;

        static auto preloadCache(unsigned int upperLimit) -> void;

        static auto factor(unsigned int n) -> std::vector<Factor>;

        static auto evaluate(const std::vector<Factor>&) -> unsigned int;
    };
}

// Help testing and debugging
[[maybe_unused]] auto __attribute__((visibility("default"))) operator<(const utility::Factor& left, const utility::Factor& right) -> bool;
[[maybe_unused]] auto __attribute__((visibility("default"))) operator==(const utility::Factor& left, const utility::Factor& right) -> bool;
[[maybe_unused]] auto __attribute__((visibility("default"))) operator<<(std::ostream&, const utility::Factor&) -> std::ostream&;

#endif //FACTOR_HPP
