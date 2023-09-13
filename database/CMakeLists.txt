add_library(sqlite_wrapper src/sqlite_wrapper.cpp)
target_include_directories(sqlite_wrapper PUBLIC include)

# to enable usage of signals_definitions.cpp
target_link_libraries(sqlite_wrapper PRIVATE common)