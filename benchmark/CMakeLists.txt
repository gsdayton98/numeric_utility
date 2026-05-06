

set (Boost_USE_STATIC_LIBS OFF)
find_package (Boost REQUIRED COMPONENTS CONFIG)
find_package(oscpp CONFIG)

add_executable(benchmark_isprime benchmark_isprime.cpp)
target_link_libraries (benchmark_isprime numeric_utility ${Boost_LIBRARIES} oscpp::oscpp)
target_include_directories (benchmark_isprime PRIVATE numeric_utility "${Boost_INCLUDE_DIRS}")
