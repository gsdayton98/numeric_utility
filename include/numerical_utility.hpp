#ifndef NUMUTIL_HPP
#define NUMUTIL_HPP
#include <algorithm>
#include <vector>

namespace utility {
  template<typename Number>
  Number gcd(Number a, Number b)
  {
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
  Number mulmod(Number a, Number b, const Number &modulo)
  {
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

  template<>
  unsigned long sqrt<unsigned long>(unsigned long);

  template<typename Number>
  std::vector<unsigned int> toDigits(Number n, const unsigned int base = 10)
  {
    std::vector<unsigned int> result;

    while (n != 0) {
      int digit = static_cast<int>(n % base);
      n /= base;
      result.push_back(digit);
    }
    return result;
  }


  template<typename Number>
  Number toNumber(const std::vector<unsigned int> &digits, const unsigned int base = 10U)
  {
    Number result = 0;
    Number place = 1;
    for (auto digit: digits) {
      result += digit * place;
      place *= base;
    }
    return result;
  }

  template<typename Number>
  auto millerRabin0(const Number &number) -> bool
  {
    static const std::vector<unsigned int> smallPrimes = {
      2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61
    };

    // bit mask of a bit set at each position that happens to be a prime number.
    static constexpr unsigned long smallPrimeMask =
        (1UL << 2) | (1UL << 3) | (1UL << 5) | (1UL << 7) | (1UL << 11) | (1UL << 13) |
        (1UL << 17) | (1UL << 19) | (1UL << 23) | (1UL << 29) | (1UL << 31) | (1UL << 37) | (1UL << 41) | (1UL << 43) |
        (1UL << 47) |
        (1UL << 53) | (1UL << 59) | (1UL << 61);

    // https://miller-rabin.appspot.com/
    // "Witnesses" aka “bases” for pseudo-prime tests.
    static constexpr unsigned long witnesses[] = {2UL, 325UL, 9'375UL, 28'178UL, 450'775UL, 1'795'265'022UL};

    if (number < 64) {
      if (const unsigned long pmask = 1UL << number; (pmask & smallPrimeMask) != 0) return true;
    }

    if (std::ranges::any_of(smallPrimes, [number](const unsigned int prime) { return number % prime == 0; })) return
        false;
    if (number < smallPrimes.back() * smallPrimes.back()) return true;

    // Find s and d such that our prime number = pow(2, s)*d + 1
    // number is a strong probable prime in base 'a' (aka a 'witness') if either of the following hold:
    //   pow(a, d) == 1 mod number
    //   pow(a, d*pow(2, r)) == -1 mod number for r in [0, s)
    auto d = number - 1;
    unsigned int s = 0;
    while ((d & 1) == 0) {
      ++s;
      d >>= 1;
    }

    // test number against all the bases
    for (const auto witness: witnesses) {
      if (number < witness - 1) continue;

      // witness**d == (1 mod number) or witness**d == -1 mod number.
      auto ad = powmod(witness, d, number);
      if (ad == 1 || ad == number - 1) {
        continue;
      }

      // Check witness**(d·2**r) for r in [1, s)
      bool candidate = false;
      for (unsigned int r = 0; r < s; ++r) {
        ad = (ad * ad) % number;
        if (ad == 1) {
          return false;
        }
        if (ad == number - 1) {
          candidate = true;
          break;
        }
      } // end for each r

      if (!candidate) return false;
    } // end for each witness

    return true;
  }
}
#endif // NUMUTIL_HPP
