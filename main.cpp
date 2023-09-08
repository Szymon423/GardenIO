#include "sqlite_wrapper.hpp"
#include <iostream>


int main()
{
    char *db_path = "/home/szymon/GardenIO/database/test.db";
    sqlite_wrapper test(db_path);
    int ret;
    ret = test.create_table();
}