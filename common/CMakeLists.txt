add_library(common src/shared_memory.cpp src/signals_definitions.cpp src/common_functions.cpp src/log_manager.cpp)

# to enable usage of signals_definitions.cpp
target_include_directories(common PUBLIC include)