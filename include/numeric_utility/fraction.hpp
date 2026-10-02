// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2026 Glen S. Dayton. Rights reserved according to terms of included license.
#ifndef NUMERIC_UTILITY_FRACTION_HPP
#define NUMERIC_UTILITY_FRACTION_HPP
#include <concepts>
#include <limits>
#include <numeric>
#include <ostream>
#include <stdexcept>


namespace utility {
    /**
     * A fraction of two signed integers, always in lowest terms with a positive denominator.
     *
     * The numerator carries the sign, so equal fractions have equal members and == compares them directly.
     * Neither member is ever the type's minimum value, whose magnitude doesn't fit in the type.
     * @tparam Integer A signed integer type. Class template argument deduction picks it from the constructor
     *         arguments, so Fraction{3, 12} is a Fraction<int>.
     */
    template <std::signed_integral Integer>
    class Fraction {
    public:
        /**
         * Construct numerator/denominator in lowest terms, moving a negative denominator's sign to the numerator.
         * @throws std::domain_error if the denominator is zero.
         * @throws std::overflow_error if either argument is the type's minimum value.
         */
        Fraction(Integer numerator, Integer denominator);

        /// The numerator, which carries the fraction's sign.
        [[nodiscard]] auto numerator() const -> Integer { return m_numerator; }

        /// The denominator, always positive.
        [[nodiscard]] auto denominator() const -> Integer { return m_denominator; }

        /**
         * Multiply by another fraction. Common factors are cancelled first, so it overflows only when the reduced
         * result doesn't fit.
         * @throws std::overflow_error if the result's numerator or denominator doesn't fit in the type.
         */
        auto operator*=(const Fraction& other) -> Fraction&;

        friend auto operator*(Fraction left, const Fraction& right) -> Fraction { return left *= right; }

        friend auto operator==(const Fraction&, const Fraction&) -> bool = default;

    private:
        Integer m_numerator;
        Integer m_denominator;

        static auto checkedMultiply(Integer a, Integer b) -> Integer;
    };


    template <std::signed_integral Integer>
    Fraction<Integer>::Fraction(const Integer numerator, const Integer denominator)
        : m_numerator{numerator}, m_denominator{denominator}
    {
        if (denominator == 0) throw std::domain_error("Fraction: zero denominator");
        constexpr auto minimum = std::numeric_limits<Integer>::min();
        if (numerator == minimum || denominator == minimum) {
            throw std::overflow_error("Fraction: the type's minimum value has no positive counterpart");
        }

        if (m_denominator < 0) {
            m_numerator = static_cast<Integer>(-m_numerator);
            m_denominator = static_cast<Integer>(-m_denominator);
        }
        // std::gcd uses the magnitudes, and the denominator isn't zero, so g >= 1.
        const auto g = std::gcd(m_numerator, m_denominator);
        m_numerator = static_cast<Integer>(m_numerator / g);
        m_denominator = static_cast<Integer>(m_denominator / g);
    }


    template <std::signed_integral Integer>
    auto Fraction<Integer>::operator*=(const Fraction& other) -> Fraction& {
        // Both fractions are in lowest terms, so after cancelling across them the product is too.
        const auto g1 = std::gcd(m_numerator, other.m_denominator);
        const auto g2 = std::gcd(other.m_numerator, m_denominator);
        m_numerator = checkedMultiply(static_cast<Integer>(m_numerator / g1), static_cast<Integer>(other.m_numerator / g2));
        m_denominator = checkedMultiply(static_cast<Integer>(m_denominator / g2), static_cast<Integer>(other.m_denominator / g1));
        if (m_numerator == 0) m_denominator = 1;
        return *this;
    }


    template <std::signed_integral Integer>
    auto Fraction<Integer>::checkedMultiply(const Integer a, const Integer b) -> Integer {
        Integer product;
        if (__builtin_mul_overflow(a, b, &product) || product == std::numeric_limits<Integer>::min()) {
            throw std::overflow_error("Fraction: product does not fit in the type");
        }
        return product;
    }


    /// Print the fraction as numerator/denominator, such as -3/4.
    template <std::signed_integral Integer>
    auto operator<<(std::ostream& out, const Fraction<Integer>& fraction) -> std::ostream& {
        // Unary + prints char-sized types as numbers rather than characters.
        return out << +fraction.numerator() << '/' << +fraction.denominator();
    }
}

#endif //NUMERIC_UTILITY_FRACTION_HPP
