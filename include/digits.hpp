#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-magic-numbers"
//
// Created by Glen Dayton, new account on 3/13/24.
//

#ifndef TO_DIGITS_HPP
#define TO_DIGITS_HPP
#include <vector>

auto __attribute__((visibility("default"))) toDigits(int n, int base=10) -> std::vector<int>;

[[maybe_unused]] auto __attribute__((visibility("default"))) toNumber(const std::vector<int>& digits, int base=10) -> int;
#endif //TO_DIGITS_HPP

#pragma clang diagnostic pop