#ifndef __NUMUTIL_HPP__
#define __NUMUTIL_HPP__
#include <algorithm>
#include <vector>

namespace numutil {


template<typename Number>
Number gcd(Number a, Number b) {
  if (a > b)
    std::swap(a, b);

  while (b % a) {
    Number temp = b;
    b = a;
    a = temp % a;
  }

  return a;
}


template<typename Number>
Number mulmod(Number a, Number b, const Number& modulo) {
  Number result = 0;

  a %= modulo;
  b %= modulo;
  if (b > a) std::swap(a, b);
  while (b > 0 && a > 0) {
    if (b & 1) {
      result += a;
      if (result >= modulo) result -= modulo;
    }

    a <<= 1;
    if (a >= modulo) a -= modulo;
    b >>= 1;
  }

  return result;
}

template<typename Number>
Number sqrt(Number c);

template <> unsigned long sqrt<unsigned long>(unsigned long);

template<typename Number>
std::vector<unsigned int> toDigits(Number n, const unsigned int base = 10) {
  std::vector<unsigned int> result;

  while (n != 0) {
    int digit = static_cast<int>(n % base);
    n /= base;
    result.push_back(digit);
  }
  return result;
}


template<typename Number>
Number toNumber(const std::vector<unsigned int>& digits, const unsigned int base = 10U) {
  Number result = 0;
  Number place = 1;
  for (auto digit : digits) {
    result += digit * place;
    place *= base;
  }
  return result;
}


template<typename Number>
bool millerRabin0(const Number& number);

}

#endif // __NUMUTIL_HPP__
