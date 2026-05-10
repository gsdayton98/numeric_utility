//
// Created by Glen Dayton, new account on 3/13/24.
//
#include "digits.hpp"

using utility::DefaultDigitType;
using utility::DefaultRadixType;

template auto
__attribute__((visibility("default")))
utility::toDigits<unsigned int>(unsigned int n, DefaultRadixType base) -> std::vector<DefaultDigitType>;

template auto
__attribute__((visibility("default")))
utility::toDigits<unsigned long>(unsigned long n, DefaultRadixType base) -> std::vector<DefaultDigitType>;

template auto
__attribute__((visibility("default")))
utility::toDigits<unsigned long long>(unsigned long long n, DefaultRadixType base) -> std::vector<DefaultDigitType>;


template auto
__attribute__((visibility("default")))
utility::toNumber<int>(const std::vector<DefaultDigitType>& digits, DefaultRadixType base) -> int;

template auto
__attribute__((visibility("default")))
utility::toNumber<unsigned int>(const std::vector<DefaultDigitType>& digits, DefaultRadixType base) -> unsigned int;

template auto
__attribute__((visibility("default")))
utility::toNumber<unsigned long>(const std::vector<DefaultDigitType>& digits, DefaultRadixType base) -> unsigned long;

template auto
__attribute__((visibility("default")))
utility::toNumber<long long>(const std::vector<DefaultDigitType>& digits, DefaultRadixType base) -> long long;

template auto
__attribute__((visibility("default")))
utility::toNumber<unsigned long long>(const std::vector<DefaultDigitType>& digits, DefaultRadixType base) -> unsigned long long;
