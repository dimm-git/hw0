cmake_minimum_required(VERSION 3.2)

project(ip_filter)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

set(DEFAULT_CXX_STDLIB "libc++")

set(OTUS_VERSION_MAJOR "1")
set(OTUS_VERSION_MINOR "0")

if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    message(STATUS "TRAVIS_BUILD_NUMBER: " "$ENV{TRAVIS_BUILD_NUMBER}")
    set(OTUS_VERSION_PATCH "$ENV{TRAVIS_BUILD_NUMBER}")
else()
    set(OTUS_VERSION_PATCH "0")
endif()

add_executable(ip_filter src/ip_filter.cpp)
add_executable(ip_tests tests/src/tests.cpp)
target_include_directories(ip_filter PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_options(ip_filter PUBLIC -Wall -Wextra -Wpedantic)

target_include_directories(ip_tests PUBLIC ${PROJECT_SOURCE_DIR}/include ${Boost_INCLUDE_DIR})
target_compile_options(ip_tests PUBLIC -Wall -Wextra -g -Wpedantic)
target_compile_definitions(ip_tests PUBLIC BOOST_TEST_DYN_LINK)
target_link_libraries(ip_tests pthread ${Boost_LIBRARIES})

set_target_properties(ip_filter ip_tests PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)

install(TARGETS ip_filter DESTINATION bin)

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "none")
set(CPACK_PACKAGE_VERSION_MAJOR ${OTUS_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${OTUS_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${OTUS_VERSION_PATCH})
execute_process(COMMAND "uname" "-m" OUTPUT_VARIABLE PKG_MACH)
string(STRIP ${PKG_MACH} CPACK_SYSTEM_NAME)

enable_testing()
add_test(NAME ip_test COMMAND "${PROJECT_BINARY_DIR}/ip_tests" "${PROJECT_SOURCE_DIR}/tests/data/ip_filter.tsv")

include(CPack)
