cmake_minimum_required(VERSION 4.0)
project(numerical_utility_main)

set(CMAKE_CXX_STANDARD 20)

add_executable(numerical_utility_main main.cpp)
target_include_directories(numerical_utility_main PRIVATE include)
target_link_libraries(numerical_utility_main PRIVATE numeric_utility)

add_subdirectory(lib)
