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