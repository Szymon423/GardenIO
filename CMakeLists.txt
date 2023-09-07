cmake_minimum_required(VERSION 3.0.0)

project(GardenIO VERSION 1.0.0)

set(CMAKE_C_COMPILER gcc-arm-linux-gnueabihf)
set(CMAKE_CPP_COMPILER g++-arm-linux-gnueabihf)

add_subdirectory(MQTT)
add_subdirectory(common)

add_executable(GardenIO main.cpp)