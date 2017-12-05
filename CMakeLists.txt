cmake_minimum_required(VERSION 3.2)

project(helloworld)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

set(OTUS_VERSION_MAJOR "1")
set(OTUS_VERSION_MINOR "0")

if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    message(STATUS "TRAVIS_BUILD_NUMBER: " "$ENV{TRAVIS_BUILD_NUMBER}")
    set(OTUS_VERSION_PATCH "$ENV{TRAVIS_BUILD_NUMBER}")
else()
    set(OTUS_VERSION_PATCH "0")
endif()

configure_file ("${PROJECT_SOURCE_DIR}/include/otus0.h.in" "${PROJECT_SOURCE_DIR}/include/otus0.h")

add_executable(helloworld src/otus0.cpp)
add_executable(test_version_defs tests/test_version.cpp)

target_include_directories(helloworld PUBLIC ${PROJECT_SOURCE_DIR})
target_compile_options(helloworld PUBLIC -Wall -Wextra)

target_include_directories(test_version_defs PUBLIC ${PROJECT_SOURCE_DIR} ${Boost_INCLUDE_DIR})
target_compile_options(test_version_defs PUBLIC -Wall -Wextra -g)
target_compile_definitions(test_version_defs PUBLIC BOOST_TEST_DYN_LINK)
target_link_libraries(test_version_defs pthread ${Boost_LIBRARIES})

install(TARGETS helloworld DESTINATION bin)

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "none")
set(CPACK_PACKAGE_VERSION_MAJOR ${OTUS_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${OTUS_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${OTUS_VERSION_PATCH})
execute_process(COMMAND "uname" "-m" OUTPUT_VARIABLE PKG_MACH)
string(STRIP ${PKG_MACH} CPACK_SYSTEM_NAME)

enable_testing()
add_test(NAME test_output COMMAND ${PROJECT_SOURCE_DIR}/tests/00-output.sh ${PROJECT_BINARY_DIR}/helloworld)
add_test(NAME test_version COMMAND ${PROJECT_SOURCE_DIR}/tests/01-version.sh ${PROJECT_BINARY_DIR}/helloworld)
add_test(NAME test_version_defs COMMAND ${PROJECT_BINARY_DIR}/test_version_defs)

include(CPack)
