#pragma once

#include <time.h>
#include <cstdint>
#include <string>
#include "common_functions.hpp"

#define MAX_SOURCE_NAME_LENGTH 100
#define MAX_UNIT_LENGTH 10


namespace signals
{
    enum signal_type
    {
        order,                                  // master to slaves
        info                                    // slaves to master
    };
    
    std::string type_to_string(signal_type type);

    struct analog_signal_t
    {
        time_t time;                            // timestamp
        double value;                           // signal value
        char unit[MAX_UNIT_LENGTH];             // text with unit of value
        int8_t valid;                           // 1 - valid : 0 - not valid
        char source[MAX_SOURCE_NAME_LENGTH];    // name of sending node
        signal_type type;                       // type of signal given by signal_type
        int8_t archive;                         // 1 - archive : 0 - do not archive

        operator std::string() const 
        { 
            std::string val;
            if (valid == 1) val = std::to_string(value);
            else val = std::string("?????");

            std::string ret;
            ret += "value: " + val + " " + std::string(unit);
            ret += ", from: " + std::string(source);
            ret += ", [" + utility::time_to_human(time) + "]";
            ret += ", type: " + type_to_string(type);
            ret += ", archive: " + std::to_string(archive);
            return ret;
        }
    };

    struct binary_signal_t
    {
        time_t time;                            // timestamp
        int8_t value;                           // signal value
        int8_t valid;                           // 1 - valid : 0 - not valid
        char source[MAX_SOURCE_NAME_LENGTH];    // name of sending node
        signal_type type;                       // type of signal given by signal_type
        int8_t archive;                         // 1 - archive : 0 - do not archive

        operator std::string() const 
        { 
            std::string val;
            if (valid == 1) val = std::to_string(value);
            else val = std::string("?????");
            
            std::string ret;
            ret += utility::time_to_human(time);
            ret += ", value: " + val;
            ret += ", from: " + std::string(source);
            ret += ", type: " + type_to_string(type);
            ret += ", archive: " + std::to_string(archive);
            return ret;
        }
    };

    
    std::string putOnBrackets(std::string tag, std::string value);
    std::string composeMessage(signals::analog_signal_t signal, std::string log="");
    std::string composeMessage(signals::binary_signal_t signal, std::string log="");

    std::string getByTag(std::string tag, std::string const &msg);
    void decomposeMessage(std::string msg, signals::analog_signal_t &signal);
    void decomposeMessage(std::string msg, signals::binary_signal_t &signal);
}