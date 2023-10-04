add_library(modbus src/modbus+sqlite_wrapper.cpp)
target_include_directories(modbus PUBLIC include)

# to enable usage of signals_definitions.cpp
# target_link_libraries(wind PRIVATE common)
# target_link_libraries(wind PRIVATE sqlite_wrapper)
# target_link_libraries(wind PRIVATE serial)
