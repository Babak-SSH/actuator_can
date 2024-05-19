cmake_minimum_required(VERSION 3.10)
project(actuator_can)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_STANDARD_REQUIRED True)
set(YAML_LIBRARY /usr/lib/x86_64-linux-gnu)

find_package(lcm REQUIRED)

include_directories("include/")
include_directories("types/")
include_directories("/usr/include/")
include_directories("/usr/local/include/lcm/")

add_library(socketcan SHARED
    "include/SocketCAN.h" "src/SocketCAN.cpp"
)
add_library(actuator SHARED 
    "include/Actuator.hpp" "src/Actuator.cpp"
)

add_executable(single_loop "test/single_loop.cpp")
target_link_libraries(single_loop PUBLIC actuator socketcan lcm)
target_compile_options(single_loop PUBLIC -std=c++14 -Wall -Wextra -pedantic -Wfloat-conversion -fdiagnostics-color=auto -g -O3 -fPIC)

add_executable(parallel_loops "test/parallel_loops.cpp")
target_link_libraries(parallel_loops PUBLIC actuator socketcan lcm -lpthread)
target_compile_options(parallel_loops PUBLIC -std=c++14 -Wall -Wextra -pedantic -Wfloat-conversion -fdiagnostics-color=auto -g -O3 -fPIC)

install(
    TARGETS single_loop parallel_loops
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)