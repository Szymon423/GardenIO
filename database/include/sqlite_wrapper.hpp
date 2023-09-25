#pragma once

#include "sqlite3.h"
#include "signals_definitions.hpp"

// https://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm
class sqlite_wrapper
{
private:
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    std::string db_path;
public:
    sqlite_wrapper(std::string db_path);
    ~sqlite_wrapper();
    int connect();
    int disconnect();
    int create_tables();
    int insert(signals::analog_signal_t signal);
    int insert(signals::binary_signal_t signal);
    static int callback(void *NotUsed, int argc, char **argv, char **azColName);
    
};