cmake_minimum_required(VERSION 3.25)
project(numeric_utility LANGUAGES CXX VERSION 1.0.0 DESCRIPTION "Numeric utility functions")
enable_testing()

find_package(Boost REQUIRED CONFIG)

# BOOST multiprecision supports up to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_REQUIRED ON)

include(GNUInstallDirs)
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "$ENV{HOME}" CACHE PATH "Install prefix" FORCE)
endif()

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
target_include_directories(numeric_utility PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_compile_options(numeric_utility PRIVATE -fvisibility=hidden)
set_target_properties(numeric_utility PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION 1)
set_target_properties(numeric_utility PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})
set_target_properties(numeric_utility PROPERTIES PUBLIC_HEADER "${HEADERS}")

install(TARGETS ${PROJECT_NAME} EXPORT numeric_utility
        ARCHIVE  DESTINATION "${LIBDIR}"
        LIBRARY  DESTINATION "${LIBDIR}"
        RUNTIME  DESTINATION "${BINDIR}"
        PUBLIC_HEADER DESTINATION "${INCLUDEDIR}")
install(EXPORT numeric_utility NAMESPACE utility:: DESTINATION "${LIBDIR}/cmake/numeric_utility")
export(TARGETS ${PROJECT_NAME} NAMESPACE utility:: FILE numeric_utility.cmake)

add_subdirectory(test)


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