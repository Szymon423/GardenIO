add_library(common src/shared_memory.cpp src/signals_definitions.cpp src/common_functions.cpp src/log_manager.cpp)

target_include_directories(common PUBLIC include)

# to enable usage of signals_definitions.hpp ang log.hpp
target_link_libraries(common PRIVATE serial)
target_link_libraries(common PRIVATE wind)
target_link_libraries(common PRIVATE modbusLib)