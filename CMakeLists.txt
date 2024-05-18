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

add_executable(test_actuators "test/single_loop.cpp" "test/parallel_loops.cpp")
target_link_libraries(test_actuators PUBLIC actuator socket_can lcm -lpthread)
target_compile_options(test_actuators PUBLIC -std=c++14 -Wall -Wextra -pedantic -Wfloat-conversion -fdiagnostics-color=auto -g -O3 -fPIC)
install(
    TARGETS test_actuators
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)