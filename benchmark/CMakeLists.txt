cmake_minimum_required(VERSION 3.25)
project(benchmark LANGUAGES CXX VERSION 1.1.2 DESCRIPTION "Benchmark Numeric Utilities")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(oscpp CONFIG REQUIRED)
find_package(numeric_utility CONFIG REQUIRED)

add_executable(benchmark_isprime benchmark_isprime.cpp)
target_link_libraries(benchmark_isprime PRIVATE utility::numeric_utility oscpp::oscpp)
target_compile_options(benchmark_isprime PRIVATE ${NUMERIC_UTILITY_WARNINGS})
