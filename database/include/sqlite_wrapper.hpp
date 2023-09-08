#include "sqlite3.h"
#include <iostream>

// https://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm
class sqlite_wrapper
{
private:
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    char * db_path;
public:
    sqlite_wrapper(char *db_path);
    ~sqlite_wrapper();
    int connect();
    int disconnect();
    int create_table();
    static int callback(void *NotUsed, int argc, char **argv, char **azColName);
    
};