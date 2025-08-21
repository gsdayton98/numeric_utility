cmake_minimum_required(VERSION 3.23)
project(numeric_utility LANGUAGES CXX VERSION 1.0.0)
find_package(Boost REQUIRED CONFIG)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES factor.cpp sieveprimes.cpp to_digits.cpp factors.cpp
		circular_buffer.cpp
		dynamiclibary.cpp
		factor.cpp
		File.cpp
		file_descriptor.cpp
		miller_rabin.cpp
		numerical_utility.cpp
		pow.cpp
		sieveprimes.cpp
		sysexception.cpp
		trim.cpp
		socket.cpp
		stopwatch.cpp
		to_digits.cpp)

include_directories(${Boost_INCLUDE_DIRS})

add_library(numeric_utility STATIC ${SOURCES})
set_target_properties(numeric_utility PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(numeric_utility PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})
set_target_properties(numeric_utility PROPERTIES PUBLIC_HEADER "${HEADERS}")
target_include_directories(numeric_utility PRIVATE ../include)

add_subdirectory(test)
