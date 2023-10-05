#include <modbus.h>
#include <string>
#include <vector>

#include "modbus_definitions.hpp"

#define MAX_COILS_NUMBER 1000
#define MAX_INPUTS_NUMBER 1000
#define MAX_HOLDING_REGISTERS_NUMBER 1000
#define MAX_INPUT_REGISTERS_NUMBER 1000


class Modbus
{
private:
    modbus_t* mb;
    time_t connectionInterval;
    std::string ip;
    int port;
    time_t interval;
    time_t lastReadTime;
    int returnedValue;
    bool keepReading;


    uint8_t coils[MAX_COILS_NUMBER];
    uint8_t inputs[MAX_INPUTS_NUMBER];
    uint16_t holdingRegisters[MAX_HOLDING_REGISTERS_NUMBER];
    uint16_t inputRegisters[MAX_INPUT_REGISTERS_NUMBER];

    std::vector<ModbusSignal> modbusSignals;
    std::vector<RegistersSet> continiousRegions_coils;
    std::vector<RegistersSet> continiousRegions_inputs;
    std::vector<RegistersSet> continiousRegions_holdingRegisters;
    std::vector<RegistersSet> continiousRegions_inputRegisters;

    void CreateContiniousRegistersSet();
    void ReadCoils();
    void ReadInputs();
    void ReadHoldingRegisters();
    void ReadInputRegisters();
    void InterpreteRegisters();
    void TranslateRegistersToValue(uint16_t* ptr_registers, ModbusSignal* ptr_signal);


public:
    ~Modbus() = default;
    Modbus() = default;
    void SetConnectionParams(std::string ip, int port);
    void SetConnectionInterval(time_t interval);
    void SetSignalsDefinitions(std::vector<ModbusSignal> signalsDefinitions);
    void RunInLoop();
};

