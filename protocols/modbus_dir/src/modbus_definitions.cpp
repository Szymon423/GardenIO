#include "modbus_definitions.hpp"


RegistersSet::RegistersSet() : startOffset(0), length(0)
{

}


RegistersSet::RegistersSet(int startOffset, int length) : startOffset(startOffset), length(length)
{
    
}

int DataTypeLength(ModbusDataType dataType)
{
    switch (dataType)
    {
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