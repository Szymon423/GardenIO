add_library(serial src/serial_lib.cpp)
target_include_directories(serial PUBLIC include)

# to enable usage of log.hpp
target_link_libraries(serial PRIVATE common)