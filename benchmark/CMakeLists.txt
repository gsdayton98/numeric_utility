find_package(oscpp CONFIG REQUIRED)

add_executable(benchmark_isprime benchmark_isprime.cpp)
target_link_libraries(benchmark_isprime PRIVATE numeric_utility oscpp::oscpp)
target_compile_options(benchmark_isprime PRIVATE ${NUMERIC_UTILITY_WARNINGS})
