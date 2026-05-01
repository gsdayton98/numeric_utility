
set (Boost_USE_STATIC_LIBS OFF)
find_package (Boost REQUIRED COMPONENTS unit_test_framework CONFIG)

find_package(CppUnitXLite CONFIG REQUIRED)

add_executable(test_utility test.cpp
        factor_test.cpp
        test_digits.cpp
        test_isqrt.cpp
        test_lcm_gcd.cpp
        test_miller_rabin.cpp
        test_pow.cpp
        test_sieve.cpp)

target_link_libraries (test_utility PRIVATE numeric_utility ${Boost_LIBRARIES})
target_include_directories(test_utility PRIVATE ../include)

add_test(NAME test_utility COMMAND test_utility)

include(CTest)
