cmake_minimum_required(VERSION 3.2)



set(OTUS_VERSION_MAJOR "1")
set(OTUS_VERSION_MINOR "0")

if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    message(STATUS "TRAVIS_BUILD_NUMBER: " "$ENV{TRAVIS_BUILD_NUMBER}")
    set(OTUS_VERSION_PATCH "$ENV{TRAVIS_BUILD_NUMBER}")
else()
    set(OTUS_VERSION_PATCH "0")
endif()

project(kkmeans)

find_package(dlib REQUIRED)
find_package(BLAS REQUIRED)

include(GNUInstallDirs)

add_library(kkmeans STATIC
    src/kkmeans.cpp
)

target_include_directories(kkmeans PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_options(kkmeans PUBLIC -Wall -Wextra -Wpedantic)
target_link_libraries(kkmeans dlib::dlib ${BLAS_LIBRARIES})
set_target_properties(kkmeans PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)

install(TARGETS kkmeans DESTINATION ${CMAKE_INSTALL_BINDIR})

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "none")
set(CPACK_PACKAGE_VERSION_MAJOR ${OTUS_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${OTUS_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${OTUS_VERSION_PATCH})
execute_process(COMMAND "uname" "-m" OUTPUT_VARIABLE PKG_MACH)
string(STRIP ${PKG_MACH} CPACK_SYSTEM_NAME)

include(CPack)
