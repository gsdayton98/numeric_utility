// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-
// Copyright 2025 Glen S. Dayton. Rights reserved according to terms of included license.
#include "miller_rabin.hpp"
#include "sieveprimes.hpp"
#include "stopwatch.hpp"
#include <cstdlib>
#include <iostream>

static constexpr auto UPPER_LIMIT = 1'000'000U;

static void benchSieve() {
    oscpp::StopWatch stopWatch; // NOLINT
    const utility::Sieve primes(UPPER_LIMIT);
    std::cout << "Sieve all primes to " << UPPER_LIMIT  <<  ": " << stopWatch.read() << " s  " << primes.primes().size() << " primes\n";

    stopWatch.reset();
    auto count = 0U;
    for (unsigned int i = 2; i < UPPER_LIMIT; ++i)
    {
        if (primes.isPrime(i)) { ++count; }
    }
    std::cout << "Sieve isPrime all primes to " << UPPER_LIMIT << ": " << stopWatch.read() << " s  " << count << " primes\n";

    stopWatch.reset();
    for (auto i = 2U; i < UPPER_LIMIT; ++i)
    {
        if (primes.isPrime(i)) { ++count; }
    }
    std::cout << "Sieve isPrime Numbers to " << UPPER_LIMIT << ": " << stopWatch.read() << " s  " << count << " primes\n";
}


static void benchMillerRabin() {
    const utility::Sieve primes(UPPER_LIMIT);
    auto count = 0u;
    oscpp::StopWatch stopWatch; // NOLINT
    for (auto n = 2U; n < UPPER_LIMIT; ++n) {
        const auto primeQ = millerRabin(n);
        if (primes.isPrime(n) != primeQ)
        {
            std::cout << n << " " << primeQ << " " << primes.isPrime(n) << "\n";
        }
        if (primeQ) {
            ++count;
        }
    }
    std::cout << "Miller Rabin: " << stopWatch.read() << " s  " << count << " primes (sieve "
        << primes.primes().size() << " primes)\n";

    stopWatch.reset();
    for (auto i = 2U; i < UPPER_LIMIT; ++i)
    {
        const auto primeQ = millerRabin(i);
        if (primeQ) { ++count; }
        if (primes.isPrime(i) != primeQ)
        {
            std::cout << "oops " << i << " " << primeQ << " " << primes.isPrime(i) << "\n";
            break;
        }
    }
    std::cout << stopWatch.read() << " s  " << count << " primes\n";
}


auto main(int, char* argv[]) -> int {
    int returnCode = EXIT_FAILURE;
    try {
        benchSieve();
        benchMillerRabin();

        returnCode = EXIT_SUCCESS;
    }
    catch (const std::exception& e)
    {
        std::cerr << *argv << ": Exception: " << e.what() << '\n';
    }
    return returnCode;
}