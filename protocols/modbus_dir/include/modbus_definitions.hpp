#pragma once

#include <cstdint>
#include <string>


enum class ModbusRegion
{
    COILS,
    INPUTS,
    HOLDING_REGISTERS,
    INPUT_REGISTERS
};


enum class Endian
{
    BIG,
    LITTLE
};


enum class ModbusDataType
{
    BOOL,
    UINT_16,
    INT_16,
    UINT_32,
    INT_32,
    UINT_64,
    INT_64,
    FLOAT,
    DOUBLE
};


union ModbusValue
{
    bool BOOL;
    uint16_t UINT_16;
    int16_t INT_16;
    uint32_t UINT_32;
    int32_t INT_32;
    uint64_t UINT_64;
    int64_t INT_64;
    float FLOAT;
    double DOUBLE;
};


class RegistersSet
{
public:
    int startOffset;
    int length;

    RegistersSet();
    RegistersSet(int startOffset, int length);
    ~RegistersSet();
};


class ModbusSignal
{
public:
    Endian endian;
    ModbusDataType dataType;
    ModbusValue value;
    ModbusRegion region;
    int offset;

    ModbusSignal();
    ModbusSignal(Endian endian, ModbusDataType dataType, ModbusRegion region, int offset);
};


int DataTypeLength(ModbusDataType dataType);
std::string ModbusValueToString(ModbusSignal signal);
std::string ModbusRegionToString(ModbusRegion region);
std::string ModbusDataTypeToString(ModbusDataType dataType);
std::string EndianToString(Endian endian);
std::string ModbusSignalInfo(ModbusSignal signal);