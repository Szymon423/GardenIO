#include "common_functions.hpp"

std::string utility::time_to_human(time_t time)
{
    std::tm tmm;
    localtime_r(&time, &tmm);
    char buffer[32];
    std::strftime(buffer, 32, "%Y.%m.%d %H:%M:%S", &tmm); 
    return std::string(buffer);
}

