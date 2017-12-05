cmake_minimum_required(VERSION 2.6)

PROJECT(helloworld)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

SET(OTUS_VERSION_MAJOR "1")
SET(OTUS_VERSION_MINOR "0")

if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    MESSAGE(STATUS "TRAVIS_BUILD_NUMBER: " "$ENV{TRAVIS_BUILD_NUMBER}")
    SET(OTUS_VERSION_PATCH "$ENV{TRAVIS_BUILD_NUMBER}")
else()
    SET(OTUS_VERSION_PATCH "0")
endif()

configure_file ("${PROJECT_SOURCE_DIR}/include/otus0.h.in" "${PROJECT_SOURCE_DIR}/include/otus0.h")

ADD_EXECUTABLE(helloworld src/otus0.cpp)
ADD_EXECUTABLE(test_version_defs tests/test_version.cpp)

target_include_directories(helloworld PUBLIC ${PROJECT_SOURCE_DIR})
set_target_properties(helloworld test_version_defs PROPERTIES COMPILE_OPTIONS "-Wall")
set_target_properties(test_version_defs PROPERTIES COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK)

target_include_directories(test_version_defs PUBLIC ${PROJECT_SOURCE_DIR} ${Boost_INCLUDE_DIR})
target_link_libraries(test_version_defs pthread ${Boost_LIBRARIES})

INSTALL(TARGETS helloworld DESTINATION bin)

SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "none")
SET(CPACK_PACKAGE_VERSION_MAJOR ${OTUS_VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${OTUS_VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${OTUS_VERSION_PATCH})
EXECUTE_PROCESS(COMMAND "uname" "-m" OUTPUT_VARIABLE PKG_MACH)
string(STRIP ${PKG_MACH} CPACK_SYSTEM_NAME)

enable_testing()
add_test(NAME test_output COMMAND ${PROJECT_SOURCE_DIR}/tests/00-output.sh ${PROJECT_BINARY_DIR}/helloworld)
add_test(NAME test_version COMMAND ${PROJECT_SOURCE_DIR}/tests/01-version.sh ${PROJECT_BINARY_DIR}/helloworld)
add_test(NAME test_version_defs COMMAND ${PROJECT_BINARY_DIR}/test_version_defs)

INCLUDE(CPack)
