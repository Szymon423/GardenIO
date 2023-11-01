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


ModbusOrder::ModbusOrder(int newSignalNumber, bool newValue)
{
    
    signalNumber = newSignalNumber;
    value.BOOL = newValue;
}


ModbusOrder::ModbusOrder(int newSignalNumber, uint16_t newValue)
{
    signalNumber = newSignalNumber;
    value.UINT_16 = newValue;
}


ModbusOrder::ModbusOrder(int newSignalNumber, int16_t newValue)
{
    signalNumber = newSignalNumber;
    value.INT_16 = newValue;
}


ModbusOrder::ModbusOrder(int newSignalNumber, uint32_t newValue)
{
    signalNumber = newSignalNumber;
    value.UINT_32 = newValue;
}


ModbusOrder::ModbusOrder(int newSignalNumber, int32_t newValue)
{
    signalNumber = newSignalNumber;
    value.INT_32 = newValue;
}


ModbusOrder::ModbusOrder(int newSignalNumber, uint64_t newValue)
{
    signalNumber = newSignalNumber;
    value.UINT_64 = newValue;
}


ModbusOrder::ModbusOrder(int newSignalNumber, int64_t newValue)
{
    signalNumber = newSignalNumber;
    value.INT_64 = newValue;
}


ModbusOrder::ModbusOrder(int newSignalNumber, float newValue)
{
    signalNumber = newSignalNumber;
    value.FLOAT = newValue;
}


ModbusOrder::ModbusOrder(int newSignalNumber, double newValue)
{
    signalNumber = newSignalNumber;
    value.DOUBLE = newValue;
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


ModbusRegion StringToModbusRegion(std::string region)
{
    if (region == "COILS") return ModbusRegion::COILS;
    if (region == "INPUTS") return ModbusRegion::INPUTS;
    if (region == "HOLDING_REGISTERS") return ModbusRegion::HOLDING_REGISTERS;
    if (region == "INPUT_REGISTERS") return ModbusRegion::INPUT_REGISTERS;
    return ModbusRegion::UNKNOWN;
}


ModbusDataType StringToModbusDataType(std::string dataType)
{
    if (dataType == "BOOL") return ModbusDataType::BOOL;
    if (dataType == "UINT_16") return ModbusDataType::UINT_16;
    if (dataType == "INT_16") return ModbusDataType::INT_16;
    if (dataType == "UINT_32") return ModbusDataType::UINT_32;
    if (dataType == "INT_32") return ModbusDataType::INT_32;
    if (dataType == "UINT_64") return ModbusDataType::UINT_64;
    if (dataType == "INT_64") return ModbusDataType::INT_64;
    if (dataType == "FLOAT") return ModbusDataType::FLOAT;
    if (dataType == "DOUBLE") return ModbusDataType::DOUBLE;
    return ModbusDataType::UNKNOWN;
}


Endian StringToEndian(std::string endian)
{
    if (endian == "BIG") return Endian::BIG;
    if (endian == "LITTLE") return Endian::LITTLE;
    return Endian::NONE;
}


void TranslateRegistersToValue(uint16_t* ptr_registers, ModbusSignal* ptr_signal)
{
    std::vector<int> order32big = { 2, 3, 0, 1 };
    std::vector<int> order32little = { 1, 0, 3, 2 };
    std::vector<int> order64big = { 6, 7, 4, 5, 2, 3, 0, 1 };
    std::vector<int> order64little = { 1, 0, 3, 2, 5, 4, 7, 6 };
    
    switch (ptr_signal->dataType)
    {
        case ModbusDataType::UINT_16:
        {
            ptr_signal->value.UINT_16 = ptr_registers[0];
            break;
        }
        case ModbusDataType::INT_16:
        {
            ptr_signal->value.INT_16 = *((int16_t*)ptr_registers);
            break;
        }
        case ModbusDataType::UINT_32:
        {
            if (ptr_signal->endian == Endian::BIG)
            {
                ptr_signal->value.UINT_32 = SwapBytesInOrder<uint32_t>(ptr_registers, order32big);
            }
            else
            {
                ptr_signal->value.UINT_32 = SwapBytesInOrder<uint32_t>(ptr_registers, order32little);
            }
            break;
        }
        case ModbusDataType::INT_32:
        {
            if (ptr_signal->endian == Endian::BIG)
            {
                ptr_signal->value.INT_32 = SwapBytesInOrder<int32_t>(ptr_registers, order32big);
            }
            else
            {
                ptr_signal->value.INT_32 = SwapBytesInOrder<int32_t>(ptr_registers, order32little);
            }
            break;
        }
        case ModbusDataType::FLOAT:
        {
            uint32_t temp_uint32;
            if (ptr_signal->endian == Endian::BIG)
            {
                temp_uint32 = SwapBytesInOrder<uint32_t>(ptr_registers, order32big);
                ptr_signal->value.FLOAT = *((float*)&temp_uint32);   
            }
            else
            {
                temp_uint32 = SwapBytesInOrder<uint32_t>(ptr_registers, order32little);
                ptr_signal->value.FLOAT = *((float*)&temp_uint32); 
            }
            break;
        }
        case ModbusDataType::UINT_64:
        {
            if (ptr_signal->endian == Endian::BIG)
            {
                ptr_signal->value.UINT_64 = SwapBytesInOrder<uint64_t>(ptr_registers, order64big);
            }
            else
            {
                ptr_signal->value.UINT_64 = SwapBytesInOrder<uint64_t>(ptr_registers, order64little);
            }
            break;
        }
        case ModbusDataType::INT_64:
        {
            if (ptr_signal->endian == Endian::BIG)
            {
                ptr_signal->value.INT_64 = SwapBytesInOrder<int64_t>(ptr_registers, order64big); 
            }
            else
            {
                ptr_signal->value.INT_64 = SwapBytesInOrder<int64_t>(ptr_registers, order64little);
            }
            break;
        }
        case ModbusDataType::DOUBLE:
        {
            uint64_t temp_uint64;
            if (ptr_signal->endian == Endian::BIG)
            {
                temp_uint64 = SwapBytesInOrder<uint64_t>(ptr_registers, order64big);
                ptr_signal->value.DOUBLE = *((double*)&temp_uint64);     
            }
            else
            {
                temp_uint64 = SwapBytesInOrder<uint64_t>(ptr_registers, order64little);
                ptr_signal->value.DOUBLE = *((double*)&temp_uint64);  
            }
            break;
        }
        default:
        {
            break;
        }
    }
}


void TranslateRegistersToValue(uint8_t* ptr_registers, ModbusSignal* ptr_signal)
{
    switch (ptr_signal->dataType)
    {
        case ModbusDataType::BOOL:
        {
            ptr_signal->value.UINT_16 = ptr_registers[0];
            break;
        }
    }
}


uint16_t* TranslateValueToRegisters(ModbusSignal& signal, ModbusValue orderValue)
{
    std::vector<int> order32big = { 2, 3, 0, 1 };
    std::vector<int> order32little = { 1, 0, 3, 2 };
    std::vector<int> order64big = { 6, 7, 4, 5, 2, 3, 0, 1 };
    std::vector<int> order64little = { 1, 0, 3, 2, 5, 4, 7, 6 };

    uint16_t* ptr = new uint16_t[DataTypeLength(signal.dataType)];


    switch (signal.dataType)
    {
        case ModbusDataType::UINT_16:
        {
            *ptr = orderValue.UINT_16;
            break;
        }
        case ModbusDataType::INT_16:
        {
            *ptr = orderValue.INT_16;
            break;
        }
        case ModbusDataType::UINT_32:
        {
            uint32_t value; 
            if (signal.endian == Endian::BIG)
            {
                value = SwapBytesInOrder<uint32_t>((uint16_t*)&orderValue.UINT_32, order32big);
            }
            else
            {
                value = SwapBytesInOrder<uint32_t>((uint16_t*)&orderValue.UINT_32, order32little);
            }
            std::memcpy(ptr, &value, 4);
            break;
        }
        case ModbusDataType::INT_32:
        {
            int32_t value;
            if (signal.endian == Endian::BIG)
            {
                value = SwapBytesInOrder<int32_t>((uint16_t*)&orderValue.INT_32, order32big);
            }
            else
            {
                value = SwapBytesInOrder<int32_t>((uint16_t*)&orderValue.INT_32, order32little);
            }
            std::memcpy(ptr, &value, 4);
            break;
        }
        case ModbusDataType::FLOAT:
        {
            uint32_t temp_uint32;
            if (signal.endian == Endian::BIG)
            {
                temp_uint32 = SwapBytesInOrder<uint32_t>((uint16_t*)&orderValue.FLOAT, order32big);
            }
            else
            {
                temp_uint32 = SwapBytesInOrder<uint32_t>((uint16_t*)&orderValue.FLOAT, order32little);
            }
            std::memcpy(ptr, &temp_uint32, 4);
            break;
        }
        case ModbusDataType::UINT_64:
        {
            uint64_t value; 
            if (signal.endian == Endian::BIG)
            {
                value = SwapBytesInOrder<uint64_t>((uint16_t*)&orderValue.UINT_64, order64big);
            }
            else
            {
                value = SwapBytesInOrder<uint64_t>((uint16_t*)&orderValue.UINT_64, order64little);
            }
            std::memcpy(ptr, &value, 8);
            break;
        }
        case ModbusDataType::INT_64:
        {
            int64_t value;
            if (signal.endian == Endian::BIG)
            {
                value = SwapBytesInOrder<int64_t>((uint16_t*)&orderValue.INT_64, order64big);
            }
            else
            {
                value = SwapBytesInOrder<int64_t>((uint16_t*)&orderValue.INT_64, order64little);
            }
            std::memcpy(ptr, &value, 8);
            break;
        }
        case ModbusDataType::DOUBLE:
        {
            uint64_t temp_uint64;
            if (signal.endian == Endian::BIG)
            {
                temp_uint64 = SwapBytesInOrder<uint64_t>((uint16_t*)&orderValue.DOUBLE, order64big);
            }
            else
            {
                temp_uint64 = SwapBytesInOrder<uint64_t>((uint16_t*)&orderValue.DOUBLE, order64little);
            }
            std::memcpy(ptr, &temp_uint64, 8);
            break;
        }
        default:
        {
            break;
        }
    }
    return ptr;
}
