#pragma once

#include <time.h>
#include <cstdint>

#define MAX_SOURCE_NAME_LENGTH 100
#define MAX_UNIT_LENGTH 10



enum signal_type
{
    order,                                  // master to slaves
    info                                    // slaves to master
};

struct analog_signal_t
{
    time_t time;                            // timestamp
    double value;                           // signal value
    char unit[MAX_UNIT_LENGTH];             // text with unit of value
    int8_t valid;                           // 1 - valid : 0 - not valid
    char source[MAX_SOURCE_NAME_LENGTH];    // name of sending node
    signal_type type;                       // type of signal given by signal_type
    int8_t archive;                         // 1 - archive : 0 - do not archive
};

struct binary_signal_t
{
    time_t time;                            // timestamp
    int8_t value;                           // signal value
    int8_t valid;                           // 1 - valid : 0 - not valid
    char source[MAX_SOURCE_NAME_LENGTH];    // name of sending node
    signal_type type;                       // type of signal given by signal_type
    int8_t archive;                         // 1 - archive : 0 - do not archive
};