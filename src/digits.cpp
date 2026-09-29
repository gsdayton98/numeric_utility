// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2024 Glen S. Dayton. Rights reserved according to terms of included license.
#include "digits.hpp"
#include "numeric_utility_export.h"

using utility::DefaultDigitType;
using utility::DefaultRadixType;

template auto
NUMERIC_UTILITY_API
utility::toDigits<unsigned int>(unsigned int n, DefaultRadixType base) -> std::vector<DefaultDigitType>;

template auto
NUMERIC_UTILITY_API
utility::toDigits<unsigned long>(unsigned long n, DefaultRadixType base) -> std::vector<DefaultDigitType>;

template auto
NUMERIC_UTILITY_API
utility::toDigits<unsigned long long>(unsigned long long n, DefaultRadixType base) -> std::vector<DefaultDigitType>;


template auto
NUMERIC_UTILITY_API
utility::toNumber<int>(const std::vector<DefaultDigitType>& digits, DefaultRadixType base) -> int;

template auto
NUMERIC_UTILITY_API
utility::toNumber<unsigned int>(const std::vector<DefaultDigitType>& digits, DefaultRadixType base) -> unsigned int;

template auto
NUMERIC_UTILITY_API
utility::toNumber<unsigned long>(const std::vector<DefaultDigitType>& digits, DefaultRadixType base) -> unsigned long;

template auto
NUMERIC_UTILITY_API
utility::toNumber<long long>(const std::vector<DefaultDigitType>& digits, DefaultRadixType base) -> long long;

template auto
NUMERIC_UTILITY_API
utility::toNumber<unsigned long long>(const std::vector<DefaultDigitType>& digits, DefaultRadixType base) -> unsigned long long;
