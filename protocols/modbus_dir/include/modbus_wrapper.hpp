#pragma once

#include <modbus/modbus.h>
#include <string>
#include <vector>
#include <mutex>
#include <atomic>
#include <thread>
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
    std::atomic_bool work;
    std::thread clientThread;

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
    void ExecuteOrders();
    void ClientFunction();

public:
    ModbusClient() = default;
    ~ModbusClient();
    void SetConnectionParams(std::string newIp, int newPort);
    void SetConnectionInterval(time_t intervalTime);
    void SetSignalsDefinitions(std::vector<ModbusSignal> inputSignalsDefinitions);
    void SetSignal(ModbusOrder order);
    void RunClient();
    void StopClient();
};



class ModbusServer
{
private:
    std::mutex mtx;
    modbus_t* mb;
    modbus_mapping_t* mb_mapping;
    std::string ip;
    uint8_t *query;
    int headerLength;
    int port;
    int returnedValue;
    bool keepWorking;
    std::vector<ModbusSignal> modbusSignals;
    std::atomic_bool needToUpdate{ 0 };
    std::atomic_bool work;
    std::thread serverThread;


    void InternalUpdater();
    void SetModbusMapping();
    void InterpreteRegisters();
    void ServerFunction();

public:
    ModbusServer() = default;
    ~ModbusServer();
    void SetConnectionParams(std::string newIp, int newPort);
    void SetSignalsDefinitions(std::vector<ModbusSignal> inputSignalsDefinitions);
    void UpdateModbusSignal(int signalNumber, ModbusValue newValue);
    void UpdateMultipleModbusSignals(std::vector<int> signalNumbers, std::vector<ModbusValue> newValues);
    void UpdateAllModbusSignals(std::vector<ModbusValue> newValues);
    void RunServer();
    void StopServer();
};