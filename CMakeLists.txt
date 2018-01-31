cmake_minimum_required(VERSION 3.2)

project(matrix)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

set(OTUS_VERSION_MAJOR "1")
set(OTUS_VERSION_MINOR "0")

if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    message(STATUS "TRAVIS_BUILD_NUMBER: " "$ENV{TRAVIS_BUILD_NUMBER}")
    set(OTUS_VERSION_PATCH "$ENV{TRAVIS_BUILD_NUMBER}")
else()
    set(OTUS_VERSION_PATCH "0")
endif()

add_executable(matrix src/matrix.cpp include/matrix.h include/composite.h include/index_proxy.h include/matrix_iterator.h)
add_executable(matrix_tests tests/src/tests.cpp)
target_include_directories(matrix PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_options(matrix PUBLIC -Wall -Wextra -Wpedantic)

target_include_directories(matrix_tests PUBLIC ${PROJECT_SOURCE_DIR}/include ${Boost_INCLUDE_DIR})
target_compile_options(matrix_tests PUBLIC -Wall -Wextra -g -Wpedantic)
target_compile_definitions(matrix_tests PUBLIC BOOST_TEST_DYN_LINK)
target_link_libraries(matrix_tests pthread ${Boost_LIBRARIES})

set_target_properties(matrix matrix_tests PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)

install(TARGETS matrix DESTINATION bin)

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "none")
set(CPACK_PACKAGE_VERSION_MAJOR ${OTUS_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${OTUS_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${OTUS_VERSION_PATCH})
execute_process(COMMAND "uname" "-m" OUTPUT_VARIABLE PKG_MACH)
string(STRIP ${PKG_MACH} CPACK_SYSTEM_NAME)

enable_testing()
add_test(NAME matrix_test COMMAND "${PROJECT_BINARY_DIR}/matrix_tests")

include(CPack)
