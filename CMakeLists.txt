cmake_minimum_required(VERSION 4.0)


project(numeric_utility_main LANGUAGES CXX VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_REQUIRED ON)

add_executable(numeric_utility_main main.cpp)
target_include_directories(numeric_utility_main PRIVATE . include)
target_link_libraries(numeric_utility_main PRIVATE numeric_utility)

add_subdirectory(lib)
