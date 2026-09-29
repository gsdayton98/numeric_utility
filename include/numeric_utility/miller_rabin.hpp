// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2025 Glen S. Dayton. Rights reserved according to terms of included license.
#ifndef NUMERIC_UTILITY_MILLER_RABIN_HPP
#define NUMERIC_UTILITY_MILLER_RABIN_HPP
#include <cstdint>
#include "numeric_utility_export.h"
namespace utility {
    NUMERIC_UTILITY_API auto millerRabin(std::uint64_t n) -> bool;
}
#endif //NUMERIC_UTILITY_MILLER_RABIN_HPP
