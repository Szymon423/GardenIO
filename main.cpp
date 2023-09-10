#include "sqlite_wrapper.hpp"
#include "signals_definitions.hpp"
#include <iostream>


int main()
{
    analog_signal_t test_struct;
    const char *db_path {"/home/szymon/GardenIO/database/test.db"};
    sqlite_wrapper test(db_path);
    int ret;
    ret = test.create_table();
}