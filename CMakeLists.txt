cmake_minimum_required(VERSION 3.2)

set(OTUS_VERSION_MAJOR "1")
set(OTUS_VERSION_MINOR "0")

if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    message(STATUS "TRAVIS_BUILD_NUMBER: " "$ENV{TRAVIS_BUILD_NUMBER}")
    set(OTUS_VERSION_PATCH "$ENV{TRAVIS_BUILD_NUMBER}")
else()
    set(OTUS_VERSION_PATCH "0")
endif()

project(async VERSION "${OTUS_VERSION_MAJOR}.${OTUS_VERSION_MINOR}.${OTUS_VERSION_PATCH}")

find_package(Threads REQUIRED)
include(GNUInstallDirs)

add_library(async SHARED include/async.h src/async.cpp
    include/async_config.h src/async_config.cpp
    include/block_builder.h src/block_builder.cpp
    include/block_listener.h src/block_listener.cpp
    include/block_shared.h
    include/build_state.h src/build_state.cpp
    include/command.h src/command.cpp
    include/command_block.h src/command_block.cpp
    include/command_printer.h src/command_printer.cpp
    include/evil_queue.h src/evil_queue.cpp
    include/input_handler.h src/input_handler.cpp
    include/logname_generator.h src/logname_generator.cpp
)

add_executable(bulkmt src/bulk.cpp)
add_executable(async_cli src/async_cli.cpp)
target_include_directories(async PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(async_cli PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(bulkmt PUBLIC ${PROJECT_SOURCE_DIR}/include)
# :FIXME: cmake adds -fPIC automatically
target_compile_options(async PUBLIC -Wall -Wextra -Wpedantic -g)
target_compile_options(async_cli PUBLIC -Wall -Wextra -Wpedantic)
target_compile_options(bulkmt PUBLIC -Wall -Wextra -Wpedantic)
target_link_libraries(async_cli async Threads::Threads)
target_link_libraries(bulkmt async Threads::Threads)
set_target_properties(async async_cli bulkmt PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)

set_target_properties(async PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(async PROPERTIES SOVERSION ${OTUS_VERSION_MAJOR})
install(TARGETS async DESTINATION ${CMAKE_INSTALL_LIBDIR})

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "none")
set(CPACK_PACKAGE_VERSION_MAJOR ${OTUS_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${OTUS_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${OTUS_VERSION_PATCH})
execute_process(COMMAND "uname" "-m" OUTPUT_VARIABLE PKG_MACH)
string(STRIP ${PKG_MACH} CPACK_SYSTEM_NAME)

enable_testing()
add_test(NAME input COMMAND "${PROJECT_SOURCE_DIR}/tests/maketests.sh" "${PROJECT_BINARY_DIR}/bulkmt")
add_test(NAME stats COMMAND "${PROJECT_SOURCE_DIR}/tests/maketests_stats.sh" "${PROJECT_BINARY_DIR}/bulkmt")

include(CPack)
