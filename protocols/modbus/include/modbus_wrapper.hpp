#include <modbus.h>
#include <string>
#include <vector>

#define MAX_COILS_NUMBER 1000
#define MAX_INPUTS_NUMBER 1000
#define MAX_HOLDING_REGISTERS_NUMBER 1000
#define MAX_INPUT_REGISTERS_NUMBER 1000


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

class ModbusSignal
{
    Endian endian;
    ModbusDataType dataType;
    ModbusValue value;
};


class Modbus
{
private:
    modbus_t* mb;
    time_t connectionInterval;
    std::string ip;
    int port;

    uint8_t coils[MAX_COILS_NUMBER];
    uint8_t inputs[MAX_INPUTS_NUMBER];
    uint16_t holdingRegisters[MAX_HOLDING_REGISTERS_NUMBER];
    uint16_t inputRegisters[MAX_INPUT_REGISTERS_NUMBER];

    std::vector<ModbusSignal> modbusSignals;

    void ReadCoils();
    void ReadInputs();
    void ReadHoldingRegisters();
    void ReadInputRegisters();


public:
    ~Modbus() = default;
    Modbus() = default;
    void SetConnectionParams(std::string ip, int port);
    void SetConnectionInterval(time_t interval);
    void SetSignalsDefinitions(std::vector<ModbusSignal> signalsDefinitions);
    void RunInLoop();

};

