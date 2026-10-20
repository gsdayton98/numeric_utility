// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2022 Glen S. Dayton. Rights reserved according to terms of included license.
#include <numeric_utility/sieveprimes.hpp>

// These come before sievePrimes, which uses Sieve<unsigned long>. GCC ignores the export attribute on an
// explicit instantiation that follows an implicit one, leaving the symbols hidden.
template class NUMERIC_UTILITY_API utility::Sieve<unsigned int>;
template class NUMERIC_UTILITY_API utility::Sieve<unsigned long>;
template class NUMERIC_UTILITY_API utility::Sieve<unsigned long long>;
