cmake_minimum_required(VERSION 3.2)

project(allocator)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

set(OTUS_VERSION_MAJOR "1")
set(OTUS_VERSION_MINOR "0")

if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    message(STATUS "TRAVIS_BUILD_NUMBER: " "$ENV{TRAVIS_BUILD_NUMBER}")
    set(OTUS_VERSION_PATCH "$ENV{TRAVIS_BUILD_NUMBER}")
else()
    set(OTUS_VERSION_PATCH "0")
endif()

add_executable(allocator src/app.cpp include/map_alloc.h include/dummyc.h)
target_include_directories(allocator PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_options(allocator PUBLIC -Wall -Wextra -Wpedantic)
set_target_properties(allocator PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)

install(TARGETS allocator DESTINATION bin)

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "none")
set(CPACK_PACKAGE_VERSION_MAJOR ${OTUS_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${OTUS_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${OTUS_VERSION_PATCH})
execute_process(COMMAND "uname" "-m" OUTPUT_VARIABLE PKG_MACH)
string(STRIP ${PKG_MACH} CPACK_SYSTEM_NAME)

include(CPack)
