cmake_minimum_required(VERSION 3.1)

project(GardenIO VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CPP_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall")
# set(CMAKE_C_COMPILER gcc-arm-linux-gnueabihf)
# set(CMAKE_CPP_COMPILER g++-arm-linux-gnueabihf)

add_subdirectory(common)
add_subdirectory(database)
add_subdirectory(protocols)
add_subdirectory(external/spdlog)

add_executable(GardenIO main.cpp)

find_package(spdlog)
find_package(nlohmann_json 3.2.0 REQUIRED)


target_link_libraries(GardenIO LINK_PUBLIC sqlite3)
target_link_libraries(GardenIO LINK_PUBLIC mosquitto)
target_link_libraries(GardenIO LINK_PUBLIC modbus)
target_link_libraries(GardenIO PUBLIC spdlog)
target_link_libraries(GardenIO PUBLIC common)
target_link_libraries(GardenIO PUBLIC sqlite_wrapper)
target_link_libraries(GardenIO PUBLIC mqtt)
target_link_libraries(GardenIO PUBLIC modbusLib)
target_link_libraries(GardenIO PUBLIC serial)
target_link_libraries(GardenIO PUBLIC wind)
