#include "signals_definitions.hpp"
#include "log.hpp"
// #include <bits/stdc++.h>
#include <cstring>


std::string signals::actionTypeToString(actionType type)
{
    switch (type)
    {
        case signals::actionType::order:
            return std::string("0");
        case signals::actionType::info:
            return std::string("1");
        default:
            return std::string("2");
    }
}


std::string signals::actionTypeToStringHuman(actionType type)
{
    switch (type)
    {
        case signals::actionType::order:
            return std::string("order");
        case signals::actionType::info:
            return std::string("info");
        default:
            return std::string("unknown");
    }
}


signals::actionType signals::stringToActionType(std::string typeString)
{
    if (typeString.find("0") != std::string::npos) return signals::actionType::order;
    else if (typeString.find("1") != std::string::npos) return signals::actionType::info;
    else return signals::actionType::unknownAction;
}


std::string signals::signalTypeToString(signals::signalType type)
{
    switch (type)
    {
        case signals::signalType::Binary:
            return std::string("0");
        case signals::signalType::Analog:
            return std::string("1");
        default:
            return std::string("2");
    }
}


std::string signals::signalTypeToStringHuman(signals::signalType type)
{
    switch (type)
    {
        case signals::signalType::Binary:
            return std::string("Binary");
        case signals::signalType::Analog:
            return std::string("Analog");
        default:
            return std::string("Unknown");
    }
}


signals::signalType signals::stringToSignalType(std::string typeString)
{
    if (typeString.find("0") != std::string::npos) return signals::signalType::Binary;
    else if (typeString.find("1") != std::string::npos) return signals::signalType::Analog;
    else return signals::signalType::UnknownSignal;
}


signals::Signal::Signal(signals::signalType _type, time_t _time, signals::Value _value, std::string _unit, int8_t _valid, std::string _source, signals::actionType _action, int8_t _archive)
{
    type = _type;
    time = _time;
    value = _value;
    unit = _unit;
    valid = _valid;
    source = _source;
    action = _action;
    archive = _archive;
}


signals::Signal::Signal(signals::signalType _type, time_t _time, double _value, std::string _unit, int8_t _valid, std::string _source, signals::actionType _action, int8_t _archive)
{
    type = _type;
    time = _time;
    value.Analog = _value;
    unit = _unit;
    valid = _valid;
    source = _source;
    action = _action;
    archive = _archive;
}


signals::Signal::Signal(signals::signalType _type, time_t _time, int8_t _value, std::string _unit, int8_t _valid, std::string _source, signals::actionType _action, int8_t _archive)
{
    type = _type;
    time = _time;
    value.Binary = _value;
    unit = _unit;
    valid = _valid;
    source = _source;
    action = _action;
    archive = _archive;
}


std::string signals::putOnBrackets(std::string tag, std::string value)
{
    return "<" + tag + ">" + value + "</" + tag + ">";
}


std::string signals::composeMessage(signals::Signal signal, std::string log)
{
    std::string msg = "";
    msg += signals::putOnBrackets("src", signal.source);
    msg += signals::putOnBrackets("type", signals::signalTypeToString(signal.type));
    msg += signals::putOnBrackets("log", log);
    msg += signals::putOnBrackets("time", std::to_string(signal.time));
    if (signal.type == signals::signalType::Analog)
    {
        msg += signals::putOnBrackets("value", std::to_string(signal.value.Analog));
        msg += signals::putOnBrackets("unit", signal.unit);
    }
    else 
    {
        msg += signals::putOnBrackets("value", std::to_string(signal.value.Binary));
    }
    msg += signals::putOnBrackets("valid", std::to_string(signal.valid));
    msg += signals::putOnBrackets("action", signals::actionTypeToString(signal.action));
    msg += signals::putOnBrackets("archive", std::to_string(signal.archive));
    return signals::putOnBrackets("msg", msg);
}


std::string signals::getByTag(std::string tag, std::string &msg)
{
    std::size_t startPos = msg.find("<" + tag + ">") + 2 + tag.length();
    std::size_t endPos = msg.find("</" + tag + ">");
    if (startPos != std::string::npos && endPos != std::string::npos)
    {
        return msg.substr(startPos, endPos - startPos);
    }
    else
    {
        return "";
    }
}


signals::Signal signals::decomposeMessage(std::string msg)
{
    std::string source = signals::getByTag("src", msg);
    signals::signalType sType = signals::stringToSignalType(signals::getByTag("type", msg));
    std::string log = signals::getByTag("log", msg);
    
    signals::signalType type = signals::stringToSignalType(signals::getByTag("type", msg));
        
    std::time_t time;
    try
    {
        time = (time_t)std::stoll(signals::getByTag("time", msg), nullptr, 10);
    }
    catch(...)
    {
        LOG_ERROR("Can't convert time string to time_t");
        time = 0;
    }

    uint8_t valid = 0;
    try
    {
        valid = std::stoi(signals::getByTag("valid", msg));
    }
    catch(...)
    {
        LOG_ERROR("Can't convert valid string to uint8_t");
    }

    signals::actionType action = signals::stringToActionType(signals::getByTag("action", msg));
    
    uint8_t archive = 0;
    try
    {
        archive = std::stoi(signals::getByTag("archive", msg));
    }
    catch(...)
    {
        LOG_ERROR("Can't convert archive string to uint8_t");
    }

    signals::Value value;
    switch (type)
    {
        case signals::signalType::Analog:
        {
            try
            {
                value.Analog = std::stod(signals::getByTag("value", msg));
            }
            catch(...)
            {
                LOG_ERROR("Can't convert value string to double");
            }
            break;
        }
        case signals::signalType::Binary:
        {
            try
            {
                value.Binary = std::stoi(signals::getByTag("value", msg));
            }
            catch(...)
            {
                LOG_ERROR("Can't convert value string to uint8_t");
            }
            break;
        }
    }

    std::string unit = signals::getByTag("unit", msg);

    signals::Signal signal(type, time, value, unit, valid, source, action, archive);
    return signal; 
}