cmake_minimum_required(VERSION 3.14)

project(GardenIO VERSION 1.0.0)

set(CMAKE_C_COMPILER gcc-arm-linux-gnueabihf)
set(CMAKE_CPP_COMPILER g++-arm-linux-gnueabihf)
set(LIBS_TO_LINK -lm -lsqlite3 -lpthreads)

add_subdirectory(common)
add_subdirectory(database)
add_subdirectory(third-party/spdlog)

add_executable(GardenIO main.cpp)

target_link_libraries(GardenIO LINK_PUBLIC sqlite3)
target_link_libraries(GardenIO PUBLIC sqlite_wrapper)
target_link_libraries(GardenIO PUBLIC common)
target_link_libraries(GardenIO PUBLIC spdlog)

# target_link_libraries(GardenIO PUBLIC ${LIBS_TO_LINK})