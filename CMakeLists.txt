cmake_minimum_required(VERSION 3.2)

project(vge)

set(OTUS_VERSION_MAJOR "1")
set(OTUS_VERSION_MINOR "0")

if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    message(STATUS "TRAVIS_BUILD_NUMBER: " "$ENV{TRAVIS_BUILD_NUMBER}")
    set(OTUS_VERSION_PATCH "$ENV{TRAVIS_BUILD_NUMBER}")
else()
    set(OTUS_VERSION_PATCH "0")
endif()

add_executable(vge src/editor.cpp include/point.h include/shape.h)
target_include_directories(vge PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_options(vge PUBLIC -Wall -Wextra -Wpedantic)

set_target_properties(vge PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)

install(TARGETS vge DESTINATION bin)

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
