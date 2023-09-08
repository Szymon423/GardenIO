#include "sqlite_wrapper.hpp"
#include <stdio.h>
#include <stdlib.h>

sqlite_wrapper::sqlite_wrapper(char *db_path)
{
    this->db_path = db_path;
}

sqlite_wrapper::~sqlite_wrapper()
{
    sqlite3_close(db);
}

int sqlite_wrapper::connect()
{
    rc = sqlite3_open(db_path, &db);

    if (rc)
    {
        std::cout << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return -1;
    } 
    else
    {
        std::cout << "Opened database successfully" << std::endl;
    }
    return 0;
}

int sqlite_wrapper::disconnect()
{
    sqlite3_close(db);
    return 0;
}

int sqlite_wrapper::create_table()
{
   int ret = this->connect();
   if (ret != 0) return -1;

   char *sql;
    /* Create SQL statement */
   sql = "CREATE TABLE COMPANY("  \
         "ID INT PRIMARY KEY     NOT NULL," \
         "NAME           TEXT    NOT NULL," \
         "AGE            INT     NOT NULL," \
         "ADDRESS        CHAR(50)," \
         "SALARY         REAL );";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, sqlite_wrapper::callback, 0, &zErrMsg);

    if (rc != SQLITE_OK)
    {
        std::cout << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
        this->disconnect();
        return -1;
    }
    else
    {
        std::cout << "Table created successfully" << std::endl;
    }

    ret = this->disconnect();
    if (ret != 0) return -1;

    return 0;
}

int sqlite_wrapper::callback(void *NotUsed, int argc, char **argv, char **azColName) 
{
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}