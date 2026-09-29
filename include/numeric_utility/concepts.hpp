// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2026 Glen S. Dayton. Rights reserved according to terms of included license.
#ifndef NUMERIC_UTILITY_CONCEPTS_HPP
#define NUMERIC_UTILITY_CONCEPTS_HPP
#include <concepts>
#include <limits>
#include <type_traits>

namespace utility {

    /// An unsigned integer type other than bool.
    /// Defined by std::numeric_limits rather than std::is_unsigned: the standard forbids
    /// specializing type traits for other types, but allows specializing numeric_limits, as
    /// Boost.Multiprecision does for its fixed-width unsigned types. is_specialized excludes
    /// types numeric_limits knows nothing about.
    template <typename T>
    concept Unsigned = std::numeric_limits<T>::is_specialized
                       && std::numeric_limits<T>::is_integer
                       && ! std::numeric_limits<T>::is_signed
                       && ! std::same_as<std::remove_cv_t<T>, bool>;

    /// A type whose arithmetic wraps around rather than overflowing, such as the unsigned integers.
    template <typename T>
    concept ModuloOverflow = std::numeric_limits<T>::is_modulo;
}

#endif //NUMERIC_UTILITY_CONCEPTS_HPP
