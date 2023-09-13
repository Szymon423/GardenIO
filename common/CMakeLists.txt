include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(common src/shared_memory.cpp src/signals_definitions.cpp)

# to enable usage of signals_definitions.cpp
target_include_directories(common PUBLIC include)