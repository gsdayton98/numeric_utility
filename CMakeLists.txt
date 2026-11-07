cmake_minimum_required(VERSION 3.25)
project(numeric_utility LANGUAGES CXX VERSION 2.1.0 DESCRIPTION "Numeric utility functions")

# Build the tests only when numeric_utility is the top-level project, so consumers using FetchContent don't need Boost.
option(NUMERIC_UTILITY_BUILD_TESTING "Build the numeric_utility tests (requires Boost.Test)" ${PROJECT_IS_TOP_LEVEL})
# The library installs outside the loader path by default (see the presets), so on macOS give the installed copy an
# absolute install name. Embedding projects don't install it, so this defaults to the top-level project only.
option(NUMERIC_UTILITY_ABSOLUTE_INSTALL_NAME "Give the installed library an absolute install name on macOS" ${PROJECT_IS_TOP_LEVEL})

include(GNUInstallDirs)
# The presets choose the prefix: `debug` installs to $HOME for development, `release` to /usr/local for delivery.
# Without a preset, a top-level build still defaults to $HOME so a plain install needs no privileges. Pass --prefix
# or -DCMAKE_INSTALL_PREFIX= to choose another. Never override the prefix of a parent project that embeds numeric_utility.
if(PROJECT_IS_TOP_LEVEL AND CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "$ENV{HOME}" CACHE PATH "Install prefix" FORCE)
endif()

find_package(Boost REQUIRED CONFIG)

# BOOST multiprecision supports up to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES src/amicable_numbers.cpp
        src/digits.cpp
        src/dijkstra.cpp
        src/factor.cpp
        src/matrix.cpp
        src/miller_rabin.cpp
        src/pow.cpp
        src/sieveprimes.cpp)

set(HEADERS include/numeric_utility/amicable_numbers.hpp
        include/numeric_utility/concepts.hpp
        include/numeric_utility/continued_fraction.hpp
        include/numeric_utility/digits.hpp
        include/numeric_utility/digits_multiprecision.hpp
        include/numeric_utility/dijkstra.hpp
        include/numeric_utility/factor.hpp
        include/numeric_utility/fraction.hpp
        include/numeric_utility/isqrt.hpp
        include/numeric_utility/lcm_gcd.hpp
        include/numeric_utility/matrix.hpp
        include/numeric_utility/miller_rabin.hpp
        include/numeric_utility/pow.hpp
        include/numeric_utility/pow_multiprecision.hpp
        include/numeric_utility/sieveprimes.hpp
        include/numeric_utility/totient.hpp)

# Build the library with the same sanitizers as its consumers, e.g. -DNUMERIC_UTILITY_SANITIZERS="address;undefined".
# An uninstrumented library under an instrumented program yields ASan container-overflow false positives.
set(NUMERIC_UTILITY_SANITIZERS "" CACHE STRING "Sanitizers to build the library, tests, and benchmarks with")
if(NUMERIC_UTILITY_SANITIZERS)
    list(JOIN NUMERIC_UTILITY_SANITIZERS "," SANITIZE_LIST)
    add_compile_options(-fsanitize=${SANITIZE_LIST} -fno-omit-frame-pointer)
    add_link_options(-fsanitize=${SANITIZE_LIST})
    # ASan doesn't see out-of-bounds vector<bool> access within an allocated word; the
    # standard library's own checks do. Each library ignores the other's macro.
    add_compile_definitions(_LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_DEBUG _GLIBCXX_ASSERTIONS)
endif()

# Warnings for the library and its tests. -Wconversion catches the silent truncations that caused earlier bugs.
set(NUMERIC_UTILITY_WARNINGS -Wall -Wextra -Wconversion)
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # CMake omits /usr/local/include from -isystem as an implicit directory, which would let Boost's own
    # warnings through. GCC already treats it as a system directory.
    list(APPEND NUMERIC_UTILITY_WARNINGS --system-header-prefix=boost/)
endif()
option(NUMERIC_UTILITY_WERROR "Treat warnings as errors" OFF)
if(NUMERIC_UTILITY_WERROR)
    list(APPEND NUMERIC_UTILITY_WARNINGS -Werror)
endif()

add_library(numeric_utility SHARED ${HEADERS} ${SOURCES})
target_include_directories(numeric_utility PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
# The *_multiprecision.hpp headers include Boost, so consumers need its include path.
target_link_libraries(numeric_utility PUBLIC Boost::headers)
target_compile_options(numeric_utility PRIVATE -fvisibility=hidden ${NUMERIC_UTILITY_WARNINGS})
# Generates numeric_utility_export.h, which defines NUMERIC_UTILITY_API for the exported non-template symbols.
include(GenerateExportHeader)
generate_export_header(numeric_utility
        EXPORT_MACRO_NAME NUMERIC_UTILITY_API
        EXPORT_FILE_NAME numeric_utility/numeric_utility_export.h)
set_target_properties(numeric_utility PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(numeric_utility PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})
# The library installs to the user's home directory, which is on no loader path. On macOS an absolute install
# name in the installed copy lets programs find it without DYLD_LIBRARY_PATH or an rpath of their own. The
# build tree keeps @rpath. Linux has no equivalent; see the README.
if(NUMERIC_UTILITY_ABSOLUTE_INSTALL_NAME)
    set_target_properties(numeric_utility PROPERTIES INSTALL_NAME_DIR "${CMAKE_INSTALL_FULL_LIBDIR}")
endif()
set_target_properties(numeric_utility PROPERTIES PUBLIC_HEADER "${HEADERS};${CMAKE_CURRENT_BINARY_DIR}/numeric_utility/numeric_utility_export.h")

install(TARGETS ${PROJECT_NAME} EXPORT numeric_utility
        ARCHIVE  DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        LIBRARY  DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        RUNTIME  DESTINATION "${CMAKE_INSTALL_BINDIR}"
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/numeric_utility")
install(EXPORT numeric_utility NAMESPACE utility:: DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/numeric_utility")
export(TARGETS ${PROJECT_NAME} NAMESPACE utility:: FILE numeric_utility.cmake)

if(NUMERIC_UTILITY_BUILD_TESTING)
    include(CTest)
    add_subdirectory(test)
endif()

# Benchmarks need the oscpp library for its StopWatch.
option(NUMERIC_UTILITY_BENCHMARKS "Build the benchmarks; needs the oscpp package" OFF)
if(NUMERIC_UTILITY_BENCHMARKS)
    add_subdirectory(benchmark)
endif()

# API documentation: cmake --build <dir> --target docs. Needs Doxygen, and network access to fetch the theme.
option(NUMERIC_UTILITY_DOCS "Add a 'docs' target that builds the API documentation with Doxygen" OFF)
if(NUMERIC_UTILITY_DOCS)
    add_subdirectory(docs)
endif()

#Packaging support
set(CPACK_PACKAGE_VENDOR "Glen Dayton")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Numeric Utilities")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
include(CPack)

include(CMakePackageConfigHelpers)
configure_package_config_file(
        "cmake/numeric_utility-config.cmake.in"
        "numeric_utility-config.cmake"
        INSTALL_DESTINATION
        "${CMAKE_INSTALL_LIBDIR}/cmake/numeric_utility")

write_basic_package_version_file(
        "numeric_utility-config-version.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/numeric_utility-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/numeric_utility-config-version.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/numeric_utility")