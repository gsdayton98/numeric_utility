// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2025 Glen S. Dayton. Rights reserved according to terms of included license.
#ifndef MILLER_RABIN_HPP
#define MILLER_RABIN_HPP
#include <cstdint>
namespace utility {
    auto __attribute__((visibility("default"))) millerRabin(std::uint64_t n) -> bool;
}
#endif //MILLER_RABIN_HPP
