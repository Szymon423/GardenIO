#include "signals_definitions.hpp"


std::string signals::type_to_string(signal_type type)
{
    switch (type)
    {
        case signal_type::order:
            return std::string("order");
        case signal_type::info:
            return std::string("info");
        default:
            return std::string("__unknown_type__");
    }
}


std::string signals::putOnBrackets(std::string tag, std::string value)
{
    return "<" + tag + ">" + value + "</" + tag + ">";
}


std::string signals::composeMessage(signals::analog_signal_t signal, std::string log="")
{
    std::string msg = "";
    msg += signals::putOnBrackets("src", signal.source);
    msg += signals::putOnBrackets("type", "0");
    msg += signals::putOnBrackets("log", log);
    msg += signals::putOnBrackets("time", std::to_string(signal.time));
    msg += signals::putOnBrackets("value", std::to_string(signal.value));
    msg += signals::putOnBrackets("unit", signal.unit);
    msg += signals::putOnBrackets("valid", std::to_string(signal.valid));
    msg += signals::putOnBrackets("action", signals::type_to_string(signal.type));
    msg += signals::putOnBrackets("archive", std::to_string(signal.archive));
    return signals::putOnBrackets("msg", msg);
}

std::string signals::composeMessage(signals::binary_signal_t signal, std::string log="")
{
    std::string msg = "";
    msg += signals::putOnBrackets("src", signal.source);
    msg += signals::putOnBrackets("type", "1");
    msg += signals::putOnBrackets("log", log);
    msg += signals::putOnBrackets("time", std::to_string(signal.time));
    msg += signals::putOnBrackets("value", std::to_string(signal.value));
    msg += signals::putOnBrackets("unit", "");
    msg += signals::putOnBrackets("valid", std::to_string(signal.valid));
    msg += signals::putOnBrackets("action", signals::type_to_string(signal.type));
    msg += signals::putOnBrackets("archive", std::to_string(signal.archive));
    return signals::putOnBrackets("msg", msg);
}


void signals::decomposeMessage(std::string msg, signals::analog_signal_t &signal)
{

}


void signals::decomposeMessage(std::string msg, signals::binary_signal_t &signal)
{

}