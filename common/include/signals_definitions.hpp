#pragma once

#include <time.h>
#include <cstdint>
#include <string>
#include "common_functions.hpp"
#include "log.hpp"

#define MAX_SOURCE_NAME_LENGTH 100
#define MAX_UNIT_LENGTH 10


namespace signals
{
    enum class signalType
    {
        Binary,
        Analog,
        UnknownSignal
    };
    
    std::string signalTypeToString(signalType type);
    std::string signalTypeToStringHuman(signalType type);
    signalType stringToSignalType(std::string typeString);

    enum class actionType
    {
        order,                                  // master to slaves
        info,                                   // slaves to master
        unknownAction                                 // some problems occured during actions
    };

    union Value
    {
        int8_t Binary;
        double Analog;
    };

    std::string actionTypeToString(actionType type);
    std::string actionTypeToStringHuman(actionType type);
    actionType stringToActionType(std::string typeString);

    class Signal
    {
    public:
        signalType type;                        // determines type of signal
        time_t time;                            // timestamp
        Value value;                            // signal value
        int8_t valid;                           // 1 - valid : 0 - not valid
        std::string unit;                       // text with unit of value
        std::string source;                     // name of sending node
        actionType action;                      // type of signal given by actionType
        int8_t archive;                         // 1 - archive : 0 - do not archive

        operator std::string() const 
        { 
            std::string val;
            switch (type)
            {
                case signals::signalType::Analog:
                {
                    if (valid == 1) val = std::to_string(value.Analog);
                    else val = std::string("?????");
                    break;
                }
                case signals::signalType::Binary:
                {
                    if (valid == 1) val = std::to_string(value.Binary);
                    else val = std::string("?????");
                    break;
                }
            }
                       

            std::string ret;
            ret += "value: " + val + " " + unit;
            ret += ", from: " + source;
            ret += ", type: " + actionTypeToString(action);
            ret += ", archive: " + std::to_string(archive);
            return ret;
        }
        ~Signal() = default;
        Signal(signals::signalType _type, time_t _time, signals::Value _value, std::string _unit, int8_t _valid, std::string _source, actionType _action, int8_t _archive);
        Signal(signals::signalType _type, time_t _time, double _value, std::string _unit, int8_t _valid, std::string _source, actionType _action, int8_t _archive);
        Signal(signals::signalType _type, time_t _time, int8_t _value, std::string _unit, int8_t _valid, std::string _source, actionType _action, int8_t _archive);
    };

    std::string putOnBrackets(std::string tag, std::string value);
    std::string composeMessage(signals::Signal signal, std::string log="");

    std::string getByTag(std::string tag, std::string &msg);
    signals::Signal decomposeMessage(std::string msg);
}