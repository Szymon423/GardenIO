cmake_minimum_required(VERSION 3.1)

project(GardenIO VERSION 1.0.0)

set(CMAKE_C_COMPILER gcc-arm-linux-gnueabihf)
set(CMAKE_CPP_COMPILER g++-arm-linux-gnueabihf)

add_subdirectory(common)
add_subdirectory(database)
add_subdirectory(mqtt)
add_subdirectory(third-party/spdlog)

add_executable(GardenIO main.cpp)

target_link_libraries(GardenIO LINK_PUBLIC sqlite3)
target_link_libraries(GardenIO LINK_PUBLIC mosquitto)
target_link_libraries(GardenIO PUBLIC sqlite_wrapper)
target_link_libraries(GardenIO PUBLIC common)
target_link_libraries(GardenIO PUBLIC mqtt)
target_link_libraries(GardenIO PUBLIC spdlog)
