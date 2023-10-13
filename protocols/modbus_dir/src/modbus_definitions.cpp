#include "modbus_definitions.hpp"
#include <string>
#include <mutex>


ModbusSignal::ModbusSignal()
{
}


ModbusSignal::ModbusSignal(Endian endian, ModbusDataType dataType, ModbusRegion region, int offset)
    : endian(endian), dataType(dataType), region(region), offset(offset)
{
}


ModbusSignal::ModbusSignal(ModbusDataType dataType, ModbusRegion region, int offset)
    : endian(Endian::NONE), dataType(dataType), region(region), offset(offset)
{
}


RegistersSet::RegistersSet() : startOffset(0), length(0)
{
}


RegistersSet::RegistersSet(int startOffset, int length) : startOffset(startOffset), length(length)
{
}


RegistersSet::~RegistersSet()
{
}


ModbusOrder::~ModbusOrder()
{

}


ModbusOrder::ModbusOrder()
{

}


ModbusOrder::ModbusOrder(int newSignalNumber, ModbusValue newValue)
    : signalNumber(newSignalNumber), value(newValue)
{
}


int DataTypeLength(ModbusDataType dataType)
{
    switch (dataType)
    {
        case ModbusDataType::BOOL: return 1;
        case ModbusDataType::UINT_16: return 1;
        case ModbusDataType::INT_16: return 1;
        case ModbusDataType::UINT_32: return 2;
        case ModbusDataType::INT_32: return 2;
        case ModbusDataType::UINT_64: return 4;
        case ModbusDataType::INT_64: return 4;
        case ModbusDataType::FLOAT: return 2;
        case ModbusDataType::DOUBLE: return 4;
        default: return 2;
    }
}


std::string ModbusValueToString(ModbusSignal signal)
{
    switch (signal.dataType)
    {
        case ModbusDataType::BOOL: return std::to_string(signal.value.BOOL);
        case ModbusDataType::UINT_16: return std::to_string(signal.value.UINT_16);
        case ModbusDataType::INT_16: return std::to_string(signal.value.INT_16);
        case ModbusDataType::UINT_32: return std::to_string(signal.value.UINT_32);
        case ModbusDataType::INT_32: return std::to_string(signal.value.INT_32);
        case ModbusDataType::UINT_64: return std::to_string(signal.value.UINT_64);
        case ModbusDataType::INT_64: return std::to_string(signal.value.INT_64);
        case ModbusDataType::FLOAT: return std::to_string(signal.value.FLOAT);
        case ModbusDataType::DOUBLE: return std::to_string(signal.value.DOUBLE);
        default: return "";
    }
}


std::string ModbusRegionToString(ModbusRegion region)
{
    switch (region)
    {
        case ModbusRegion::INPUTS: return "Inputs";
        case ModbusRegion::COILS: return "Coils";
        case ModbusRegion::HOLDING_REGISTERS: return "Holding Registers";
        case ModbusRegion::INPUT_REGISTERS: return "Input Registers";
        default: return "Unknown";
    }
}


std::string ModbusDataTypeToString(ModbusDataType dataType)
{
    switch (dataType)
    {
        case ModbusDataType::BOOL: return "BOOL";
        case ModbusDataType::UINT_16: return "UINT_16";
        case ModbusDataType::INT_16: return "INT_16";
        case ModbusDataType::UINT_32: return "UINT_32";
        case ModbusDataType::INT_32: return "INT_32";
        case ModbusDataType::UINT_64: return "UINT_64";
        case ModbusDataType::INT_64: return "INT_64";
        case ModbusDataType::FLOAT: return "FLOAT";
        case ModbusDataType::DOUBLE: return "DOUBLE";
        default: return "Unknown";
    }
}


std::string EndianToString(Endian endian)
{
    switch (endian)
    {
        case Endian::BIG: return "Big endian";
        case Endian::LITTLE: return "Little endian";
        default: return "Unknown";
    }
}


std::string ModbusSignalInfo(ModbusSignal signal)
{
    std::string info = "";
    info += ModbusRegionToString(signal.region) + " - ";
    info += ModbusDataTypeToString(signal.dataType) + " - ";
    info += EndianToString(signal.endian) + " - ";
    info += "offset: " + std::to_string(signal.offset);
    return info;
}