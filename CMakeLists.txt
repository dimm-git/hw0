cmake_minimum_required(VERSION 3.2)

project(bulkmt)

find_package(Threads REQUIRED)

set(OTUS_VERSION_MAJOR "1")
set(OTUS_VERSION_MINOR "0")

if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    message(STATUS "TRAVIS_BUILD_NUMBER: " "$ENV{TRAVIS_BUILD_NUMBER}")
    set(OTUS_VERSION_PATCH "$ENV{TRAVIS_BUILD_NUMBER}")
else()
    set(OTUS_VERSION_PATCH "0")
endif()

add_executable(bulkmt src/bulk.cpp
    include/app_config.h src/app_config.cpp
    include/app_mode.h src/app_mode.cpp
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

target_include_directories(bulkmt PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_options(bulkmt PUBLIC -Wall -Wextra -Wpedantic)
target_link_libraries(bulkmt Threads::Threads)
set_target_properties(bulkmt PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)

install(TARGETS bulkmt DESTINATION bin)

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "none")
set(CPACK_PACKAGE_VERSION_MAJOR ${OTUS_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${OTUS_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${OTUS_VERSION_PATCH})
execute_process(COMMAND "uname" "-m" OUTPUT_VARIABLE PKG_MACH)
string(STRIP ${PKG_MACH} CPACK_SYSTEM_NAME)

enable_testing()
add_test(NAME input COMMAND "${PROJECT_SOURCE_DIR}/tests/maketests.sh" "${PROJECT_BINARY_DIR}/bulkmt")

include(CPack)
