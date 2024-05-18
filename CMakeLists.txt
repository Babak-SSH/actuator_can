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