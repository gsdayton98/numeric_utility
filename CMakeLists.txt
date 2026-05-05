cmake_minimum_required(VERSION 3.25)
project(numeric_utility LANGUAGES CXX VERSION 1.0.0 DESCRIPTION "Numeric utility functions")
enable_testing()

find_package(Boost REQUIRED CONFIG)

# BOOST multiprecision supports up to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_REQUIRED ON)


set(BINDIR bin)
set(LIBDIR lib)
set(INCLUDEDIR include)

set(SOURCES src/amicable_numbers.cpp
        src/digits.cpp
        src/factor.cpp
        src/factors.cpp
        src/miller_rabin.cpp
        src/numerical_utility.cpp
        src/pow.cpp
        src/sieveprimes.cpp)

set(HEADERS include/amicable_numbers.hpp
        include/digits.hpp
        include/factor.hpp
        include/factors.hpp
        include/isqrt.hpp
        include/lcm_gcd.hpp
        include/miller_rabin.hpp
        include/numerical_utility.hpp
        include/pow.hpp
        include/sieveprimes.hpp)

add_library(numeric_utility SHARED ${HEADERS} ${SOURCES})
target_include_directories(numeric_utility PRIVATE ./include)
target_compile_options(numeric_utility PRIVATE -fvisibility=hidden)
set_target_properties(numeric_utility PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION 1)
set_target_properties(numeric_utility PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})
set_target_properties(numeric_utility PROPERTIES PUBLIC_HEADER "${HEADERS}")
install(TARGETS ${PROJECT_NAME} EXPORT numeric_utility
        ARCHIVE  DESTINATION "${LIBDIR}"
        LIBRARY  DESTINATION "${LIBDIR}"
        RUNTIME  DESTINATION "${BINDIR}"
        PUBLIC_HEADER DESTINATION "${INCLUDEDIR}")
install(EXPORT numeric_utility DESTINATION share/oscpp/cmake)
export(TARGETS ${PROJECT_NAME} FILE numeric_utility.cmake)


add_subdirectory(test)
