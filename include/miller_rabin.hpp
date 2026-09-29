// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2025 Glen S. Dayton. Rights reserved according to terms of included license.
#ifndef MILLER_RABIN_HPP
#define MILLER_RABIN_HPP
#include <cstdint>
#include "numeric_utility_export.h"
namespace utility {
    NUMERIC_UTILITY_API auto millerRabin(std::uint64_t n) -> bool;
}
#endif //MILLER_RABIN_HPP
