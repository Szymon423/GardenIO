add_library(modbusLib src/modbus_wrapper.cpp src/modbus_definitions.cpp)
target_include_directories(modbusLib PUBLIC include)

# to enable usage of signals_definitions.cpp
# target_link_libraries(wind PRIVATE common)
# target_link_libraries(wind PRIVATE sqlite_wrapper)
# target_link_libraries(wind PRIVATE serial)
