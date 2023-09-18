add_library(wind src/wind_sensor.cpp)
target_include_directories(wind PUBLIC include)

# to enable usage of signals_definitions.cpp
target_link_libraries(wind PRIVATE common)
