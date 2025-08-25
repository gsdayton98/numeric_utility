#ifndef POW_HPP_
#define POW_HPP_
#include <numeric>

template <typename T>
concept ModuloOverflow = std::numeric_limits<T>::is_modulo;

template <typename T>
concept Unsigned = ! std::numeric_limits<T>::is_signed;


namespace utility {
    ////
    /// Pow(base, exponent)target_link_libraries(test_socket utility ${Boost_LIBRARIES})

    /// Evaluate base**exponent. base and exponent must be unsigned or at least non-negative.
    /// Beware, that the result is explicitly modulo the size of the unsigned type, for example,
    /// base 32-bit unsigned int is evaluated modulo 2**32.

    template<ModuloOverflow BaseType>
    BaseType pow(BaseType base, BaseType exponent) {
        BaseType result = 1;

        if (base != 0) {
            while (exponent > 0) {
                if (exponent & 1) result *= base;
                base *= base;
                exponent >>= 1;
            }
        }

        return result;
    }


    ////
    /// powmod(base, exponent, modulus)
    /// Evaluate base**exponent modulo modulus.
    /// This version should only be used with unsigned types.
    template<Unsigned BaseType>
    auto powmod(BaseType base, BaseType exponent, const BaseType& modulus) -> BaseType {
        if (modulus < 2) return 0;
        BaseType result = 1;
        base %= modulus;
        while (exponent > 0)
        {
            if (exponent & 1) result = (result * base) % modulus;
            base = (base * base) % modulus;
            exponent >>= 1;
        }
        return result;
    }
}
#endif
