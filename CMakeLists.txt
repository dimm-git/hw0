cmake_minimum_required(VERSION 3.2)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREAD ON)

set(OTUS_VERSION_MAJOR "1")
set(OTUS_VERSION_MINOR "0")

if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    message(STATUS "TRAVIS_BUILD_NUMBER: " "$ENV{TRAVIS_BUILD_NUMBER}")
    set(OTUS_VERSION_PATCH "$ENV{TRAVIS_BUILD_NUMBER}")
else()
    set(OTUS_VERSION_PATCH "0")
endif()

project(join_server)

find_package(Boost COMPONENTS system REQUIRED)
find_package(Threads REQUIRED)
include(GNUInstallDirs)

add_library(join STATIC
    include/server.h src/server.cpp
    include/session.h src/session.cpp
)

add_executable(join_server src/join_server.cpp)

target_include_directories(join PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(join_server PUBLIC ${PROJECT_SOURCE_DIR}/include ${Boost_INCLUDE_DIRS})

target_compile_options(join PUBLIC -Wall -Wextra -Wpedantic)
target_compile_options(join_server PUBLIC -Wall -Wextra -Wpedantic)

link_directories(${Boost_LIBRARY_DIRS})
target_link_libraries(join_server join Boost::system Threads::Threads)
# target_link_libraries(bulkmt async Threads::Threads)

set_target_properties(join_server join PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)

install(TARGETS join_server DESTINATION ${CMAKE_INSTALL_BINDIR})

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "none")
set(CPACK_PACKAGE_VERSION_MAJOR ${OTUS_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${OTUS_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${OTUS_VERSION_PATCH})
execute_process(COMMAND "uname" "-m" OUTPUT_VARIABLE PKG_MACH)
string(STRIP ${PKG_MACH} CPACK_SYSTEM_NAME)

# enable_testing()
# add_test(NAME input COMMAND "${PROJECT_SOURCE_DIR}/tests/maketests.sh" "${PROJECT_BINARY_DIR}/bulkmt")
# add_test(NAME stats COMMAND "${PROJECT_SOURCE_DIR}/tests/maketests_stats.sh" "${PROJECT_BINARY_DIR}/bulkmt")

include(CPack)
