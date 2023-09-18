add_library(mqtt src/mqtt_client.cpp)
target_include_directories(mqtt PUBLIC include)

# to enable usage of signals_definitions.cpp
target_link_libraries(mqtt PRIVATE common)
