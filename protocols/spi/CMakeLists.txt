add_library(spi src/spi.cpp)
target_include_directories(spi PUBLIC include)

# to enable usage of spi.cpp
target_link_libraries(spi PRIVATE common)
