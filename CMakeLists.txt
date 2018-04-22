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

add_executable(kkmeans
    src/kkmeans.cpp
    src/kfunc.cpp
)

add_executable(kgen
    src/kgen.cpp
    src/kfunc.cpp
)

configure_file ("${PROJECT_SOURCE_DIR}/gnuplot/kvis.in" "${PROJECT_SOURCE_DIR}/gnuplot/kvis")

target_include_directories(kkmeans PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(kgen PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_options(kkmeans PUBLIC -Wall -Wextra -Wpedantic)
target_compile_options(kgen PUBLIC -Wall -Wextra -Wpedantic)
target_link_libraries(kkmeans dlib::dlib ${BLAS_LIBRARIES})
target_link_libraries(kgen dlib::dlib ${BLAS_LIBRARIES})
set_target_properties(kkmeans kgen PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)

install(TARGETS kkmeans kgen DESTINATION ${CMAKE_INSTALL_BINDIR})
install(
    FILES
	${PROJECT_SOURCE_DIR}/gnuplot/gp2
	${PROJECT_SOURCE_DIR}/gnuplot/gp3
	${PROJECT_SOURCE_DIR}/data/kkmeans_ex_class.png
	${PROJECT_SOURCE_DIR}/data//kkmeans_ex.png
	${PROJECT_SOURCE_DIR}/data/kkmeans_ex.txt
	${PROJECT_SOURCE_DIR}/data/stars_class.png
	${PROJECT_SOURCE_DIR}/data/stars_class.txt
	${PROJECT_SOURCE_DIR}/data/stars.png
	${PROJECT_SOURCE_DIR}/data/stars.txt
    DESTINATION
	${CMAKE_INSTALL_DATAROOTDIR}/kkmeans)
install(FILES ${PROJECT_SOURCE_DIR}/gnuplot/kvis DESTINATION ${CMAKE_INSTALL_BINDIR})

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "none")
set(CPACK_PACKAGE_VERSION_MAJOR ${OTUS_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${OTUS_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${OTUS_VERSION_PATCH})
set(CPACK_DEBIAN_PACKAGE_DEPENDS "gnuplot, feh, libblas3, libdlib18")
execute_process(COMMAND "uname" "-m" OUTPUT_VARIABLE PKG_MACH)
string(STRIP ${PKG_MACH} CPACK_SYSTEM_NAME)

include(CPack)
