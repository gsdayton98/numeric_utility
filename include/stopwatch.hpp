// -*- mode: c++ -*-
// @copyright 2021 Glen S. Dayton. All rights reserved,

//// StopWatch class
//
//  Simple utility for timing sections of code.

#ifndef STOPWATCH_HPP_INCL
#define STOPWATCH_HPP_INCL
#include <chrono>

namespace utility {
    class StopWatch {
        std::chrono::steady_clock::time_point start;

    public:
        StopWatch() : start{std::chrono::steady_clock::now()}
        {}

        auto reset() -> void
        { start = std::chrono::steady_clock::now(); }

        [[nodiscard]] auto read() const -> double;
    };
}
#endif // STOPWATCH_HPP_INCL
