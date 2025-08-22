cmake_minimum_required(VERSION 3.23)
project(numeric_utility LANGUAGES CXX VERSION 1.0.0 DESCRIPTION "Utility functions")
find_package(Boost REQUIRED CONFIG)

set(BINDIR bin)
set(LIBDIR lib)
set(INCLUDEDIR include)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES factor.cpp sieveprimes.cpp to_digits.cpp factors.cpp
		circular_buffer.cpp
		dynamiclibary.cpp
		factor.cpp
		file.cpp
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

set(HEADERS
		../include/circular_buffer.hpp
		../include/dynamiclibrary.hpp
		../include/factor.hpp
		../include/factors.hpp
		../include/file_descriptor.hpp
		../include/file.hpp
		../include/isqrt.hpp
		../include/lcm_gcd.hpp
		../include/miller_rabin.hpp
		../include/numerical_utility.hpp
		../include/pow.hpp
		../include/sieveprimes.hpp
		../include/socket.hpp
		../include/stopwatch.hpp
		../include/sysexception.hpp
		../include/to_digits.hpp
		../include/trim.hpp
)

include_directories(${Boost_INCLUDE_DIRS})

add_library(numeric_utility SHARED ${SOURCES})
set_target_properties(numeric_utility PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(numeric_utility PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})
set_target_properties(numeric_utility PROPERTIES PUBLIC_HEADER "${HEADERS}")
target_include_directories(numeric_utility PRIVATE ../include)

install(TARGETS ${PROJECT_NAME} EXPORT numeric_utility
		ARCHIVE  DESTINATION ${LIBDIR}
		LIBRARY  DESTINATION ${LIBDIR}
		RUNTIME  DESTINATION ${BINDIR}
		PUBLIC_HEADER DESTINATION ${INCLUDEDIR})
install(EXPORT numeric_utility DESTINATION share/numeric_utility/cmake)

export(TARGETS ${PROJECT_NAME} FILE numeric_utility.cmake)

add_subdirectory(test)
