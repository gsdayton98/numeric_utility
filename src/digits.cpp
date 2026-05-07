//
// Created by Glen Dayton, new account on 3/13/24.
//

#include "digits.hpp"

auto utility::toDigits(int n, int base) -> std::vector<int>
{
    std::vector<int> result;
    do {
        auto qr = std::div(n, base);
        result.push_back(qr.rem);
        n = qr.quot;
    } while (n != 0);

    return result;
}


[[maybe_unused]] auto utility::toNumber(const std::vector<int>& digits, int base) -> int
{
    int number = 0;
    int power = 1;

    for (auto digit: digits) {
        number += digit*power;
        power *= base;
    }
    return number;
}
