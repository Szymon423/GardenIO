#include "modbus_wrapper.hpp"
#include "modbus_definitions.hpp"
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <iomanip>
#include <bitset>
#include <string>
#include <sstream>
#include "log.hpp"


ModbusClient::~ModbusClient()
{
    modbus_close(mb);
    modbus_free(mb);
}


void ModbusClient::SetConnectionParams(std::string newIp, int newPort)
{
    ip = newIp;
    port = newPort;
}


void ModbusClient::SetConnectionInterval(time_t intervalTime)
{
    interval = intervalTime;
}


void ModbusClient::SetSignalsDefinitions(std::vector<ModbusSignal> inputSignalsDefinitions)
{
    mtx.lock();
    modbusSignals = inputSignalsDefinitions;
    mtx.unlock();
    LOG_TRACE("New signal definitions:");
    for (int i = 0; i < modbusSignals.size(); i++)
    {
        LOG_TRACE("{}", ModbusSignalInfo(modbusSignals.at(i)));
    }
    CreateContiniousRegistersSet();
}


void ModbusClient::RunClient()
{
    if (work)
    {
        LOG_ERROR("Modbus client is allready running, stop it firs to run it again.");
        return;
    }

    work = true;
    clientThread = std::thread(&ModbusClient::ClientFunction, &(*this));
}


void ModbusClient::ClientFunction()
{
    mb = modbus_new_tcp(ip.c_str(), port);
    if (mb == NULL)
    {
        LOG_TRACE("Unable to allocate libmodbus context");
    }

    returnedValue = modbus_connect(mb);
    if (returnedValue == -1)
    {
        LOG_WARN("Connection failed");
    }
    else
    {
        LOG_TRACE("Connected to: {}:{}", ip, port);
    }

    lastReadTime = time(NULL) - 2 * interval;
    LOG_TRACE("Starting reading loop with interval: {}ms", interval);
    while (work)
    {
        if (time(NULL) > lastReadTime + interval)
        {
            LOG_TRACE("Reading cycle");
            ExecuteOrders();
            ReadCoils();
            ReadInputs();
            ReadHoldingRegisters();
            ReadInputRegisters();
            InterpreteRegisters();
            lastReadTime = time(NULL);
        }
    }
}


void ModbusClient::CreateContiniousRegistersSet()
{
    std::vector<ModbusSignal> _coils;
    std::vector<ModbusSignal> _inputs;
    std::vector<ModbusSignal> _holdingRegisters;
    std::vector<ModbusSignal> _inputRegisters;
    
    for (int i = 0; i < modbusSignals.size(); i++)
    {
        switch (modbusSignals.at(i).region)
        {
            case ModbusRegion::COILS:
                _coils.push_back(modbusSignals.at(i));
                break;
            case ModbusRegion::INPUTS:
                _inputs.push_back(modbusSignals.at(i));
                break;
            case ModbusRegion::HOLDING_REGISTERS:
                _holdingRegisters.push_back(modbusSignals.at(i));
                break;
            case ModbusRegion::INPUT_REGISTERS:
                _inputRegisters.push_back(modbusSignals.at(i));
                break;
        }
    }

    RegistersSet set;
    for (int i = 0; i < _coils.size(); i++)
    {
        if (i == 0)
        {
            set.startOffset = _coils.at(i).offset;
            set.length = DataTypeLength(_coils.at(i).dataType);
        }
        else if (set.startOffset + set.length != _coils.at(i).offset)
        {
            continiousRegions_coils.push_back(set);
        }
        else
        {
            set.length += DataTypeLength(_coils.at(i).dataType);
        }
    }
    if (set.length != 0 && _coils.size() > 0) continiousRegions_coils.push_back(set);

    for (int i = 0; i < _inputs.size(); i++)
    {
        if (i == 0)
        {
            set.startOffset = _inputs.at(i).offset;
            set.length = DataTypeLength(_inputs.at(i).dataType);
        }
        else if (set.startOffset + set.length != _inputs.at(i).offset)
        {
            continiousRegions_inputs.push_back(set);
        }
        else
        {
            set.length += DataTypeLength(_inputs.at(i).dataType);
        }
    }
    if (set.length != 0 && _inputs.size() > 0) continiousRegions_inputs.push_back(set);

    for (int i = 0; i < _holdingRegisters.size(); i++)
    {
        if (i == 0)
        {
            set.startOffset = _holdingRegisters.at(i).offset;
            set.length = DataTypeLength(_holdingRegisters.at(i).dataType);
        }
        else if (set.startOffset + set.length != _holdingRegisters.at(i).offset)
        {
            continiousRegions_holdingRegisters.push_back(set);
        }
        else
        {
            set.length += DataTypeLength(_holdingRegisters.at(i).dataType);
        }
    }
    if (set.length != 0 && _holdingRegisters.size() > 0) continiousRegions_holdingRegisters.push_back(set);

    for (int i = 0; i < _inputRegisters.size(); i++)
    {
        if (i == 0)
        {
            set.startOffset = _inputRegisters.at(i).offset;
            set.length = DataTypeLength(_inputRegisters.at(i).dataType);
        }
        else if (set.startOffset + set.length != _inputRegisters.at(i).offset)
        {
            continiousRegions_inputRegisters.push_back(set);
        }
        else
        {
            set.length += DataTypeLength(_inputRegisters.at(i).dataType);
        }
    }
    if (set.length != 0 && _inputRegisters.size() > 0) continiousRegions_inputRegisters.push_back(set);
}


void ModbusClient::ReadCoils()
{
    for (RegistersSet& set : continiousRegions_coils)
    {
        LOG_TRACE("Reading Coils from: {} length: {}", set.startOffset, set.length)
        uint8_t* ptr = new uint8_t[set.length];
        returnedValue = modbus_read_bits(mb, set.startOffset, set.length, ptr);
        std::memcpy(&coils[set.startOffset], ptr, set.length * sizeof(uint8_t));
        delete[] ptr;
    }
}


void ModbusClient::ReadInputs()
{
    for (RegistersSet& set : continiousRegions_inputs)
    {
        LOG_TRACE("Reading Inputs from: {} length: {}", set.startOffset, set.length);
        uint8_t* ptr = new uint8_t[set.length];
        returnedValue = modbus_read_input_bits(mb, set.startOffset, set.length, ptr);
        std::memcpy(&inputs[set.startOffset], ptr, set.length * sizeof(uint8_t));
        delete[] ptr;
    }
}


void ModbusClient::ReadHoldingRegisters()
{
    for (RegistersSet& set : continiousRegions_holdingRegisters)
    {
        LOG_TRACE("Reading Holding Registers from: {} length: {}", set.startOffset, set.length)
        uint16_t* ptr = new uint16_t[set.length];
        returnedValue = modbus_read_registers(mb, set.startOffset, set.length, ptr);
        std::memcpy(&holdingRegisters[set.startOffset], ptr, set.length * sizeof(uint16_t));
        delete[] ptr;
    }
}


void ModbusClient::ReadInputRegisters()
{
    for (RegistersSet& set : continiousRegions_inputRegisters)
    {
        uint16_t* ptr = new uint16_t[set.length];
        returnedValue = modbus_read_input_registers(mb, set.startOffset, set.length, ptr);
        std::memcpy(&inputRegisters[set.startOffset], ptr, set.length * sizeof(uint16_t));
        delete[] ptr;
    }
}


void ModbusClient::InterpreteRegisters()
{
    LOG_TRACE("Interpreting {} signalns", modbusSignals.size());
    std::scoped_lock lock(mtx);
    for (int i = 0; i < modbusSignals.size(); i++)
    {
        switch (modbusSignals.at(i).region)
        {
            case ModbusRegion::COILS:
                TranslateRegistersToValue(&coils[modbusSignals.at(i).offset], &modbusSignals.at(i));
                break;
            case ModbusRegion::INPUTS:
                TranslateRegistersToValue(&inputs[modbusSignals.at(i).offset], &modbusSignals.at(i));
                break;
            case ModbusRegion::HOLDING_REGISTERS:
                TranslateRegistersToValue(&holdingRegisters[modbusSignals.at(i).offset], &modbusSignals.at(i));
                break;
            case ModbusRegion::INPUT_REGISTERS:
                TranslateRegistersToValue(&inputRegisters[modbusSignals.at(i).offset], &modbusSignals.at(i));
                break;
        }
        LOG_TRACE("Signal {}: {}", i, ModbusValueToString(modbusSignals.at(i)));
    }
}


void ModbusClient::SetSignal(ModbusOrder order)
{
    std::scoped_lock lock(mtx);
    try 
    {
        // test cast to void to check if at() throws exception
        (void)modbusSignals.at(order.signalNumber);
    }
    catch (...)
    {
        LOG_WARN("Trying to set value to signal which does not exist");
        return;
    }
    orders.push_back(order);
}


void ModbusClient::ExecuteOrders()
{
    std::scoped_lock lock(mtx);
    LOG_TRACE("Executing {} orders", orders.size());
    while (orders.size() > 0)
    {
        ModbusSignal& signal = modbusSignals.at(orders.front().signalNumber);
        switch (signal.region)
        {
            case ModbusRegion::COILS:
            {
                returnedValue = modbus_write_bit(mb, signal.offset, static_cast<int>(orders.front().value.BOOL));
                break;
            }
            case ModbusRegion::HOLDING_REGISTERS:
            {
                LOG_TRACE("writing {} registers from address {}", DataTypeLength(signal.dataType), signal.offset)
                returnedValue = modbus_write_registers(mb, signal.offset, DataTypeLength(signal.dataType), TranslateValueToRegisters(signal, orders.front().value));
                break;
            }
            default:
            {
                LOG_WARN("Order with invalid ModbusClient Region");
            }
        }
        orders.erase(orders.begin());
    }
}


void ModbusClient::StopClient()
{
    work = false;
    clientThread.join();
}

ModbusServer::~ModbusServer()
{
    modbus_mapping_free(mb_mapping);
    modbus_close(mb);
    modbus_free(mb);
}


void ModbusServer::SetModbusMapping()
{
    int coils_number = 0;
    int inputs_number = 0;
    int holdingRegisters_number = 0;
    int inputRegisters_number = 0;

    if (modbusSignals.size() < 1)
    {
        LOG_ERROR("No signals definitions found.");
        returnedValue = -1;
        return;
    }

    for (auto& signal : modbusSignals)
    {
        if (signal.region == ModbusRegion::COILS) coils_number += 1;
        else if (signal.region == ModbusRegion::INPUTS) inputs_number += 1;
        else if (signal.region == ModbusRegion::HOLDING_REGISTERS) holdingRegisters_number += DataTypeLength(signal.dataType);
        else if (signal.region == ModbusRegion::INPUT_REGISTERS) inputRegisters_number += DataTypeLength(signal.dataType);
    }

    mb_mapping = modbus_mapping_new(coils_number, inputs_number, holdingRegisters_number, inputRegisters_number);

    if (mb_mapping == NULL) 
    {
        LOG_ERROR("Failed to allocate the mapping: {}", modbus_strerror(errno));
        modbus_free(mb);
        returnedValue = -1;
    }

    LOG_TRACE("Modbus server mapping set.");
}


void ModbusServer::SetSignalsDefinitions(std::vector<ModbusSignal> inputSignalsDefinitions)
{
    mtx.lock();
    modbusSignals = inputSignalsDefinitions;
    mtx.unlock();
    LOG_TRACE("New signal definitions:");
    for (int i = 0; i < modbusSignals.size(); i++)
    {
        LOG_TRACE("{}", ModbusSignalInfo(modbusSignals.at(i)));
    }
}


void ModbusServer::UpdateModbusSignal(int signalNumber, ModbusValue newValue)
{
    if (signalNumber >= modbusSignals.size() || signalNumber < 0)
    {
        LOG_ERROR("Trying to update signal with number: {} excids defined modbusSignals.", signalNumber);
        return;
    }
    std::scoped_lock lock(mtx);
    modbusSignals.at(signalNumber).value = newValue;
    needToUpdate = true;
}


void ModbusServer::UpdateMultipleModbusSignals(std::vector<int> signalNumbers, std::vector<ModbusValue> newValues)
{
    if (signalNumbers.size() != newValues.size())
    {
        LOG_ERROR("signalNumbers size not equal to newValues size.")
        return;
    }
    std::scoped_lock lock(mtx);
    for (auto& signalNumber : signalNumbers)
    {
        if (signalNumber >= modbusSignals.size() || signalNumber < 0)
        {
            LOG_ERROR("Trying to update signal with: {} number excids defined modbusSignals.", signalNumber);
            continue;
        }
        modbusSignals.at(signalNumber).value = newValues.at(signalNumber);
    }

    needToUpdate = true;
}


void ModbusServer::UpdateAllModbusSignals(std::vector<ModbusValue> newValues)
{
    if (modbusSignals.size() != newValues.size())
    {
        LOG_ERROR("newValues size not equal to modbusSignals size.")
        return;
    }
    std::scoped_lock lock(mtx);
    for (int i = 0; i < newValues.size(); i++)
    {
        modbusSignals.at(i).value = newValues.at(i);
    }

    needToUpdate = true;
}


void ModbusServer::InternalUpdater()
{
    LOG_TRACE("Updating Modbus server data");
    std::scoped_lock lock(mtx);
    for (auto& signal : modbusSignals)
    {
        switch (signal.region)
        {
            case ModbusRegion::COILS:
            {
                mb_mapping->tab_bits[signal.offset] = (uint8_t)signal.value.BOOL;
                break;
            }
            case ModbusRegion::INPUTS:
            {
                mb_mapping->tab_input_bits[signal.offset] = (uint8_t)signal.value.BOOL;
                break;
            }
            case ModbusRegion::HOLDING_REGISTERS:
            {
                std::memcpy(&mb_mapping->tab_registers[signal.offset], TranslateValueToRegisters(signal, signal.value), 2 * DataTypeLength(signal.dataType));
                break;
            }
            case ModbusRegion::INPUT_REGISTERS:
            {
                std::memcpy(&mb_mapping->tab_input_registers[signal.offset], TranslateValueToRegisters(signal, signal.value), 2 * DataTypeLength(signal.dataType));
                break;
            }
            default:
            {
                LOG_ERROR("Unknown Modbus region in internal updater.");
            }
        }
    }
    needToUpdate = false;
}


void ModbusServer::SetConnectionParams(std::string newIp, int newPort)
{
    ip = newIp;
    port = newPort;
}


void ModbusServer::RunServer()
{
    if (work)
    {
        LOG_ERROR("Modbus server is allready running, stop it firs to run it again.");
        return;
    }

    LOG_TRACE("Starting Modbus server.");
    work = true;
    serverThread = std::thread(&ModbusServer::ServerFunction, this);
}


void ModbusServer::ServerFunction()
{
    while (work)
    {
        mb = modbus_new_tcp(NULL, port);
        if (mb == nullptr)
        {
            LOG_ERROR("Unable to start Modbus server at: {}:{}", ip, port);
            work = false;
            return;
        }
        
        headerLength = modbus_get_header_length(mb);
        SetModbusMapping();

        LOG_TRACE("Modbus server is listening on {}:{}", ip, port);
    
        int s = modbus_tcp_listen(mb, 1);
        if (s == -1)
        {
            LOG_TRACE("modbus_tcp_listen error. {}.", modbus_strerror(errno));
            modbus_free(mb);
            continue;
        }
        LOG_TRACE("s = {}", s);

        int ret = modbus_tcp_accept(mb, &s);
        if (ret == -1)
        {
            LOG_TRACE("modbus_tcp_accept erorr. {}.", modbus_strerror(errno));
            modbus_free(mb);
            close(s);   
            continue;
        }
        LOG_TRACE("Connection accepted on socket: {}.", s);

        int rc;
        // query = new uint8_t[MODBUS_TCP_MAX_ADU_LENGTH];
        uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
        while (work)
        {
            do 
            {
                if (needToUpdate)
                {
                    InternalUpdater();
                }
                rc = modbus_receive(mb, query);
            } 
            while (rc == 0 && work);

            // The connection is not closed on errors which require on reply such as bad CRC in RTU
            if (rc == -1 && errno != EMBBADCRC) 
            {
                break;
            }

            returnedValue = modbus_reply(mb, query, rc, mb_mapping);
            if (rc == -1) 
            {
                break;
            }
        }
        close(s); 
        modbus_mapping_free(mb_mapping);
        modbus_close(mb);
        modbus_free(mb);
    }
    
    LOG_TRACE("Modbus server stopped running.");
}


void ModbusServer::StopServer()
{
    LOG_TRACE("Stopping Modbus server.");
    work = false;
    serverThread.join();
}