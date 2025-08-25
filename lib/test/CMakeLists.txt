
set (Boost_USE_STATIC_LIBS OFF)
find_package (Boost REQUIRED COMPONENTS unit_test_framework CONFIG)
include_directories (../../include ${Boost_INCLUDE_DIRS})

find_package(CppUnitXLite)

add_executable(test_utility test.cpp
        test_to_digits.cpp
        factor_test.cpp
        test_sieve.cpp
        test_pow.cpp
        test_miller_rabin.cpp)

target_link_libraries (test_utility PRIVATE numeric_utility ${Boost_LIBRARIES})

add_executable(test_dynamic_library test_dynamic_library.cpp)
target_link_libraries(test_dynamic_library PRIVATE numeric_utility ${Boost_LIBRARIES})

add_executable(test_system_exception test_system_exception.cpp mock_strerror.cpp)
target_link_libraries(test_system_exception PRIVATE numeric_utility ${Boost_LIBRARIES})

add_executable(testTrim test_trim.cpp)
target_link_libraries(testTrim PRIVATE numeric_utility ${Boost_LIBRARIES})

add_executable(test_file_descriptor test_file_descriptor.cpp)
target_link_libraries(test_file_descriptor numeric_utility ${Boost_LIBRARIES})

add_executable(test_socket test_socket.cpp)
target_link_libraries(test_socket numeric_utility ${Boost_LIBRARIES})

add_executable(test_circular_buffer test_circular_buffer.cpp)
target_include_directories(test_circular_buffer AFTER PRIVATE ${CMAKE_SOURCE_DIR}/..)

add_test(NAME test_utility COMMAND test_utility)
add_test(NAME test_circular_buffer COMMAND test_circular_buffer)
add_test(NAME test_dynamic_library COMMAND test_dynamic_library)
add_test(NAME test_system_exception COMMAND test_system_exception)
add_test(NAME testTrim COMMAND testTrim)
add_test(NAME test_file_descriptor COMMAND test_file_descriptor)
add_test(NAME test_socket COMMAND test_socket)

include(CTest)
