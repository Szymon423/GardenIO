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
        case signals::signalType::binary:
            return std::string("0");
        case signals::signalType::analog:
            return std::string("1");
        default:
            return std::string("2");
    }
}


std::string signals::signalTypeToStringHuman(signals::signalType type)
{
    switch (type)
    {
        case signals::signalType::binary:
            return std::string("binary");
        case signals::signalType::analog:
            return std::string("analog");
        default:
            return std::string("unknown");
    }
}


signals::signalType signals::stringToSignalType(std::string typeString)
{
    if (typeString.find("0") != std::string::npos) return signals::signalType::analog;
    else if (typeString.find("1") != std::string::npos) return signals::signalType::binary;
    else return signals::signalType::unknownSignal;
}


signals::analog_signal_t::analog_signal_t(time_t _time, double _value, std::string _unit, int8_t _valid, std::string _source, actionType _action, int8_t _archive)
{
    this->time = _time;
    this->value = _value;
    this->unit = _unit;
    this->valid = _valid;
    this->source = _source;
    this->action = _action;
    this->archive = _archive;
}


signals::binary_signal_t::binary_signal_t(time_t _time, double _value, int8_t _valid, std::string _source, actionType _action, int8_t _archive)
{
    this->time = _time;
    this->value = _value;
    this->valid = _valid;
    this->source = _source;
    this->action = _action;
    this->archive = _archive;
}


std::string signals::putOnBrackets(std::string tag, std::string value)
{
    return "<" + tag + ">" + value + "</" + tag + ">";
}


std::string signals::composeMessage(signals::analog_signal_t signal, std::string log)
{
    std::string msg = "";
    msg += signals::putOnBrackets("src", signal.source);
    msg += signals::putOnBrackets("type", "0");
    msg += signals::putOnBrackets("log", log);
    msg += signals::putOnBrackets("time", std::to_string(signal.time));
    msg += signals::putOnBrackets("value", std::to_string(signal.value));
    msg += signals::putOnBrackets("unit", signal.unit);
    msg += signals::putOnBrackets("valid", std::to_string(signal.valid));
    msg += signals::putOnBrackets("action", signals::actionTypeToString(signal.action));
    msg += signals::putOnBrackets("archive", std::to_string(signal.archive));
    return signals::putOnBrackets("msg", msg);
}

std::string signals::composeMessage(signals::binary_signal_t signal, std::string log)
{
    std::string msg = "";
    msg += signals::putOnBrackets("src", signal.source);
    msg += signals::putOnBrackets("type", "1");
    msg += signals::putOnBrackets("log", log);
    msg += signals::putOnBrackets("time", std::to_string((long long)signal.time));
    msg += signals::putOnBrackets("value", std::to_string(signal.value));
    msg += signals::putOnBrackets("unit", "");
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


void signals::decomposeMessage(std::string msg, signals::signal_t &signal)
{
    std::string source = signals::getByTag("src", msg);
    signals::signalType sType = signals::stringToSignalType(signals::getByTag("type", msg));
    std::string log = signals::getByTag("log", msg);
    std::time_t time;
    try
    {
        LOG_TRACE("time: {}", signals::getByTag("time", msg));
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
        LOG_TRACE("valid: {}", signals::getByTag("valid", msg));
        valid = std::stoi(signals::getByTag("valid", msg));
    }
    catch(...)
    {
        LOG_ERROR("Can't convert valid string to uint8_t");
    }

    LOG_TRACE("action: {}", signals::getByTag("action", msg));
    signals::actionType aType = signals::stringToActionType(signals::getByTag("action", msg));
    
    uint8_t archive = 0;
    try
    {
        LOG_TRACE("archive: {}", signals::getByTag("archive", msg));
        archive = std::stoi(signals::getByTag("archive", msg));
    }
    catch(...)
    {
        LOG_ERROR("Can't convert archive string to uint8_t");
    }

    switch (sType)
    {
        case signals::signalType::analog:
        {
            double value;
            try
            {
                LOG_TRACE("value: {}", signals::getByTag("value", msg));
                value = std::stod(signals::getByTag("value", msg));
            }
            catch(...)
            {
                LOG_ERROR("Can't convert value string to double");
            }

            LOG_TRACE("unit: {}", signals::getByTag("unit", msg));
            std::string unit = signals::getByTag("unit", msg);

            LOG_TRACE("value double: {}", value);
            signals::analog_signal_t my_signal(time, value, unit, valid, source, aType, archive);
            signal = my_signal;
            break;
        }
        case signals::signalType::binary:
        {
            uint8_t value;
            try
            {
                LOG_TRACE("value: {}", signals::getByTag("value", msg));
                value = std::stoi(signals::getByTag("value", msg));
            }
            catch(...)
            {
                LOG_ERROR("Can't convert value string to uint8_t");
            }

            signals::binary_signal_t my_signal(time, value, valid, source, aType, archive);
            signal = my_signal;
            break;
        }
        return;
    }
}