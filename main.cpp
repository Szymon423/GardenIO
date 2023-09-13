#include "sqlite_wrapper.hpp"
#include "signals_definitions.hpp"
#include <iostream>
#include <cstring>


int main()
{
    signals::analog_signal_t my_struct;
    my_struct.time = 123456;
    my_struct.value = 9.9;
    strcpy(my_struct.unit, "none");
    my_struct.valid = 1;
    strcpy(my_struct.source, "test");
    my_struct.type = signals::signal_type::info;

    std::cout << std::string(my_struct) << std::endl;

    const char *db_path {"/home/szymon/GardenIO/database/test.db"};
    sqlite_wrapper db(db_path);
    
    int ret;
    ret = db.create_tables();
    ret = db.insert(my_struct);
}