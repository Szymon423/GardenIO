#pragma once

#include <time.h>
#include <cstdint>
#include <string>
#include "common_functions.hpp"

#define MAX_SOURCE_NAME_LENGTH 100
#define MAX_UNIT_LENGTH 10


namespace signals
{
    enum signalType
    {
        binary,
        analog,
        unknownSignal
    };

    std::string signalTypeToString(signalType type);
    std::string signalTypeToStringHuman(signalType type);
    signalType stringToSignalType(std::string typeString);
    
    enum actionType
    {
        order,                                  // master to slaves
        info,                                   // slaves to master
        unknownAction                                 // some problems occured during actions
    };
    
    std::string actionTypeToString(actionType type);
    std::string actionTypeToStringHuman(actionType type);
    actionType stringToActionType(std::string typeString);


    class signal_t
    {
    public:
        time_t time;                            // timestamp
        int8_t valid;                           // 1 - valid : 0 - not valid
        std::string source;                     // name of sending node
        actionType action;                       // type of signal given by actionType
        int8_t archive;                         // 1 - archive : 0 - do not archive
    };

    class analog_signal_t : public signal_t
    {
    public:
        
        double value;                           // signal value
        std::string unit;                       // text with unit of value

        operator std::string() const 
        { 
            std::string val;
            if (valid == 1) val = std::to_string(value);
            else val = std::string("?????");

            std::string ret;
            ret += "value: " + val + " " + unit;
            ret += ", from: " + source;
            ret += ", [" + utility::time_to_human(time) + "]";
            ret += ", type: " + actionTypeToString(action);
            ret += ", archive: " + std::to_string(archive);
            return ret;
        }

        analog_signal_t(time_t time, double value, std::string unit, int8_t valid, std::string source, actionType action, int8_t archive);
    };

    class binary_signal_t : public signal_t
    {
    public:
        int8_t value;                           // signal value  

        operator std::string() const 
        { 
            std::string val;
            if (valid == 1) val = std::to_string(value);
            else val = std::string("?????");
            
            std::string ret;
            ret += utility::time_to_human(time);
            ret += ", value: " + val;
            ret += ", from: " + source;
            ret += ", type: " + actionTypeToString(action);
            ret += ", archive: " + std::to_string(archive);
            return ret;
        }

        binary_signal_t(time_t time, double value, int8_t valid, std::string source, actionType action, int8_t archive);
    };

    
    std::string putOnBrackets(std::string tag, std::string value);
    std::string composeMessage(signals::analog_signal_t signal, std::string log="");
    std::string composeMessage(signals::binary_signal_t signal, std::string log="");

    std::string getByTag(std::string tag, std::string const &msg);
    void decomposeMessage(std::string msg, signals::signal_t &signal);
}