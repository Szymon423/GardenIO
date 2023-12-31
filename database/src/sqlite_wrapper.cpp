#include "sqlite_wrapper.hpp"
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <mutex>
#include "log.hpp"


sqlite_wrapper::sqlite_wrapper(std::string db_path) : db_path(db_path) {}


sqlite_wrapper::~sqlite_wrapper()
{
    sqlite3_close(db);
}


int sqlite_wrapper::connect()
{
    const char *path {  db_path.c_str() };
    rc = sqlite3_open(path, &db);

    if (rc)
    {
        LOG_ERROR("Can't open database: {}", sqlite3_errmsg(db));
        // std::cout << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return -1;
    } 
    else
    {
        LOG_TRACE("Opened database successfully");
        // std::cout << "Opened database successfully" << std::endl;
    }
    return 0;
}


int sqlite_wrapper::disconnect()
{
    sqlite3_close(db);
    return 0;
}


int sqlite_wrapper::create_tables()
{
    int ret = this->connect();
    if (ret != 0) return -1;

    // create ANALOGS table
    char const *sql;
    sql = "CREATE TABLE ANALOGS("  \
          "ID        INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL," \
          "time      INT         NOT NULL," \
          "value     REAL        NOT NULL," \
          "unit      CHAR(10)," \
          "valid     INT         NOT NULL," \
          "source    CHAR(100)," \
          "type      INT         NOT NULL);";

    rc = sqlite3_exec(db, sql, sqlite_wrapper::callback, 0, &zErrMsg);

    if (rc != SQLITE_OK)
    {
        LOG_ERROR("SQL error: {}", zErrMsg);
        // std::cout << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
        this->disconnect();
        return -1;
    }
    else
    {
        LOG_TRACE("Table ANALOGS created successfully");
        // std::cout << "Table ANALOGS created successfully" << std::endl;
    }

    // create BINARIES table
    sql = "CREATE TABLE BINARIES("  \
          "ID        INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL," \
          "time      INT         NOT NULL," \
          "value     INT         NOT NULL," \
          "valid     INT         NOT NULL," \
          "source    CHAR(100)," \
          "type      INT         NOT NULL);";

    rc = sqlite3_exec(db, sql, sqlite_wrapper::callback, 0, &zErrMsg);

    if (rc != SQLITE_OK)
    {
        LOG_ERROR("SQL error: {}", zErrMsg);
        // std::cout << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
        this->disconnect();
        return -1;
    }
    else
    {
        LOG_TRACE("Table BINARIES created successfully");
        // std::cout << "Table BINARIES created successfully" << std::endl;
    }

    ret = this->disconnect();
    if (ret != 0) return -1;

    return 0;
}


int sqlite_wrapper::insert(signals::Signal signal)
{
    std::scoped_lock lock(mtx);
    int ret = this->connect();
    if (ret != 0) return -1;

    // create include statement
    std::string sql_s;
    switch (signal.type)
    {
        case signals::signalType::Analog:
        {
            sql_s = "INSERT INTO ANALOGS (time,value,unit,valid,source,type) ";
            sql_s += "VALUES (" + std::to_string(signal.time) + ", ";
            sql_s += std::to_string(signal.value.Analog) + ",";
            sql_s += "\'" + signal.unit + "',";
            sql_s += std::to_string(signal.valid) + ",";
            sql_s += "\'" + signal.source + "',";
            sql_s += signals::actionTypeToString(signal.action) + ");";
            break;
        }
        case signals::signalType::Binary:
        {
            std::string sql_s = "INSERT INTO BINARIES (time,value,valid,source,type) ";
            sql_s += "VALUES (" + std::to_string(signal.time) + ", ";
            sql_s += std::to_string(signal.value.Binary) + ",";
            sql_s += std::to_string(signal.valid) + ",";
            sql_s += "\'" + signal.source + "',";
            sql_s += signals::actionTypeToString(signal.action) + ");";
            break;
        }
    } 

    char const *sql = const_cast<char*>(sql_s.c_str());

    rc = sqlite3_exec(db, sql, sqlite_wrapper::callback, 0, &zErrMsg);

    if (rc != SQLITE_OK)
    {
        LOG_ERROR("SQL error: {}", zErrMsg);
        // std::cout << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
        this->disconnect();
        return -1;
    }
    else
    {
        switch (signal.type)
        {
            case signals::signalType::Analog:
            {
                LOG_TRACE("Inserted into ANALOGS successfully");
                break;
            }
            case signals::signalType::Binary:
            {
                LOG_TRACE("Inserted into BINARIES successfully");
                break;
            }
        }    
    }

    ret = this->disconnect();
    if (ret != 0) return -1;

    return 0;
}


int sqlite_wrapper::callback(void *NotUsed, int argc, char **argv, char **azColName) 
{
    for(int i = 0; i < argc; i++)
    {
        LOG_TRACE("{} = {}", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    return 0;
}