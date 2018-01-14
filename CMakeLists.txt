cmake_minimum_required(VERSION 3.2)

project(ip_printer)

# find_package(Boost COMPONENTS unit_test_framework REQUIRED)

set(OTUS_VERSION_MAJOR "1")
set(OTUS_VERSION_MINOR "0")

if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    message(STATUS "TRAVIS_BUILD_NUMBER: " "$ENV{TRAVIS_BUILD_NUMBER}")
    set(OTUS_VERSION_PATCH "$ENV{TRAVIS_BUILD_NUMBER}")
else()
    set(OTUS_VERSION_PATCH "0")
endif()

add_executable(ip_printer src/ip_printer.cpp include/print_ip.h include/print_tuple.h include/print_ord.h include/print_container.h)
# add_executable(ip_tests tests/src/tests.cpp)
target_include_directories(ip_printer PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_options(ip_printer PUBLIC -Wall -Wextra -Wpedantic)

# target_include_directories(ip_tests PUBLIC ${PROJECT_SOURCE_DIR}/include ${Boost_INCLUDE_DIR})
# target_compile_options(ip_tests PUBLIC -Wall -Wextra -g -Wpedantic)
# target_compile_definitions(ip_tests PUBLIC BOOST_TEST_DYN_LINK)
# target_link_libraries(ip_tests pthread ${Boost_LIBRARIES})

# set_target_properties(ip_printer ip_tests PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)
set_target_properties(ip_printer PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)

install(TARGETS ip_printer DESTINATION bin)

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "none")
set(CPACK_PACKAGE_VERSION_MAJOR ${OTUS_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${OTUS_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${OTUS_VERSION_PATCH})
execute_process(COMMAND "uname" "-m" OUTPUT_VARIABLE PKG_MACH)
string(STRIP ${PKG_MACH} CPACK_SYSTEM_NAME)

if (NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "Release")

find_package(Doxygen)
set(doxyfile_in ${PROJECT_SOURCE_DIR}/doc/doxyfile.in)
set(doxyfile ${PROJECT_BINARY_DIR}/doxyfile)
configure_file(${doxyfile_in} ${doxyfile} @ONLY)

add_custom_target(doc
  COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  COMMENT "Generating API documentation with Doxygen"
  VERBATIM
)

endif()

# enable_testing()
# add_test(NAME ip_test COMMAND "${PROJECT_BINARY_DIR}/ip_tests" "${PROJECT_SOURCE_DIR}/tests/data/ip_printer.tsv")

include(CPack)
