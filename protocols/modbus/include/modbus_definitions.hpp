#include <cstdint>


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
    UINT_16,
    INT_16,
    UINT_32,
    INT_32,
    UINT64,
    INT_64,
    FLOAT,
    DOUBLE
};


union ModbusValue
{
    uint16_t UINT_16;
    int16_t INT_16;
    uint32_t UINT_32;
    int32_t INT_32;
    uint64_t UINT64;
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
    Endian endian;
    ModbusDataType dataType;
    ModbusValue value;
    ModbusRegion region;
    int offset;
};


int DataTypeLength(ModbusDataType dataType);