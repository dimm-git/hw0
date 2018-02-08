cmake_minimum_required(VERSION 3.2)

project(timus)

add_executable(p1196 src/p1196.cpp)
target_compile_options(p1196 PUBLIC -Wall -Wextra -Wpedantic)
set_target_properties(p1196 PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)

add_executable(p1496 src/p1496.cpp)
target_compile_options(p1496 PUBLIC -Wall -Wextra -Wpedantic)
set_target_properties(p1496 PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)

add_executable(p1545 src/p1545.cpp)
target_compile_options(p1545 PUBLIC -Wall -Wextra -Wpedantic)
set_target_properties(p1545 PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)

add_executable(p1837 src/p1837.cpp)
target_compile_options(p1837 PUBLIC -Wall -Wextra -Wpedantic)
set_target_properties(p1837 PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)
