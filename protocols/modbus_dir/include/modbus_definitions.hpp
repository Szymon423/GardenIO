#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <mutex>
#include <iomanip>
#include <bitset>
#include <string>
#include <sstream>

#include "log.hpp"


enum class ModbusRegion
{
    COILS,
    INPUTS,
    HOLDING_REGISTERS,
    INPUT_REGISTERS,
    UNKNOWN
};


enum class Endian
{
    BIG,
    LITTLE,
    NONE
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
    DOUBLE,
    UNKNOWN
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
    ModbusSignal(ModbusDataType dataType, ModbusRegion region, int offset);
};


class ModbusOrder
{
public:

    int signalNumber;
    ModbusValue value;
    
    ModbusOrder();
    ~ModbusOrder();
    ModbusOrder(int newSignalNumber, bool newValue);
    ModbusOrder(int newSignalNumber, uint16_t newValue);
    ModbusOrder(int newSignalNumber, int16_t newValue);
    ModbusOrder(int newSignalNumber, uint32_t newValue);
    ModbusOrder(int newSignalNumber, int32_t newValue);
    ModbusOrder(int newSignalNumber, uint64_t newValue);
    ModbusOrder(int newSignalNumber, int64_t newValue);
    ModbusOrder(int newSignalNumber, float newValue);
    ModbusOrder(int newSignalNumber, double newValue);
    ModbusOrder(int newSignalNumber, ModbusValue newValue);
};


int DataTypeLength(ModbusDataType dataType);
std::string ModbusValueToString(ModbusSignal signal);
std::string ModbusRegionToString(ModbusRegion region);
std::string ModbusDataTypeToString(ModbusDataType dataType);
std::string EndianToString(Endian endian);
std::string ModbusSignalInfo(ModbusSignal signal);

ModbusRegion StringToModbusRegion(std::string region);
ModbusDataType StringToModbusDataType(std::string dataType);
Endian StringToEndian(std::string endian);

template <typename T>
T SwapBytesInOrder(uint16_t* oryginal, std::vector<int>& order)
{
    T oryginalVlaue = *((T*)oryginal);
    T newValue = 0;    
    // std::stringstream ss;
    // ss << std::bitset<sizeof(T) * 8>{oryginalVlaue};
    // LOG_TRACE("{}", ss.str());

    uint8_t* ptr_oryginalVlaue = (uint8_t*)&oryginalVlaue;
    uint8_t* ptr_newVlaue = (uint8_t*)&newValue;
    for (int i = 0; i < sizeof(T) && order.size() == sizeof(T); i++)
    {
        ptr_newVlaue[i] = ptr_oryginalVlaue[order.at(i)];
    }
    
    return newValue;
}



void TranslateRegistersToValue(uint16_t* ptr_registers, ModbusSignal* ptr_signal);
void TranslateRegistersToValue(uint8_t* ptr_registers, ModbusSignal* ptr_signal);
uint16_t* TranslateValueToRegisters(ModbusSignal& signal, ModbusValue orderValue);