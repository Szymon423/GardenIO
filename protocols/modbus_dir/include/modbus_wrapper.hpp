#pragma once

#include <modbus/modbus.h>
#include <string>
#include <vector>
#include <mutex>
#include <atomic>
#include <time.h>
#include "modbus_definitions.hpp"

#define MAX_COILS_NUMBER 1000
#define MAX_INPUTS_NUMBER 1000
#define MAX_HOLDING_REGISTERS_NUMBER 1000
#define MAX_INPUT_REGISTERS_NUMBER 1000



class ModbusClient
{
private:
    std::mutex mtx;
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

    std::vector<ModbusOrder> orders;

    void CreateContiniousRegistersSet();
    void ReadCoils();
    void ReadInputs();
    void ReadHoldingRegisters();
    void ReadInputRegisters();
    void InterpreteRegisters();
    void TranslateRegistersToValue(uint16_t* ptr_registers, ModbusSignal* ptr_signal);
    void TranslateRegistersToValue(uint8_t* ptr_registers, ModbusSignal* ptr_signal);
    void ExecuteOrders();
    uint16_t* TranslateValueToRegisters(ModbusSignal& signal, ModbusValue orderValue);


public:
    ModbusClient() = default;
    ~ModbusClient();
    void SetConnectionParams(std::string newIp, int newPort);
    void SetConnectionInterval(time_t intervalTime);
    void SetSignalsDefinitions(std::vector<ModbusSignal> inputSignalsDefinitions);
    void RunClient();
    void SetSignal(ModbusOrder order);
};



class ModbusServer
{
private:
    std::mutex mtx;
    modbus_t* mb;
    modbus_mapping_t* mb_mapping;
    std::string ip;
    uint8_t *query;
    int headeLlength;
    int port;
    int returnedValue;
    bool keepWorking;
    std::vector<ModbusSignal> modbusSignals;
    std::atomic_bool needToUpdate{ 0 };

    void InternalUpdater();
    void SetModbusMapping();
    void InterpreteRegisters();
    void TranslateRegistersToValue(uint16_t* ptr_registers, ModbusSignal* ptr_signal);
    void TranslateRegistersToValue(uint8_t* ptr_registers, ModbusSignal* ptr_signal);
    uint16_t* TranslateValueToRegisters(ModbusSignal& signal, ModbusValue orderValue);


public:
    ModbusServer() = default;
    ~ModbusServer();
    void SetConnectionParams(std::string newIp, int newPort);
    void SetSignalsDefinitions(std::vector<ModbusSignal> inputSignalsDefinitions);
    void UpdateModbusSignal(int signalNumber, ModbusValue newValue);
    void UpdateMultipleModbusSignals(std::vector<int> signalNumbers, std::vector<ModbusValue> newValues);
    void UpdateAllModbusSignals(std::vector<ModbusValue> newValues);
    void RunServer();
};