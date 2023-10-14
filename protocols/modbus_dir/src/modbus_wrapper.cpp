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


Modbus::~Modbus()
{
    modbus_close(mb);
    modbus_free(mb);
}


void Modbus::SetConnectionParams(std::string newIp, int newPort)
{
    ip = newIp;
    port = newPort;
}


void Modbus::SetConnectionInterval(time_t intervalTime)
{
    interval = intervalTime;
}


void Modbus::SetSignalsDefinitions(std::vector<ModbusSignal> inputSignalsDefinitions)
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


void Modbus::RunInLoop()
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

    keepReading = true;
    lastReadTime = time(NULL) - 2 * interval;
    time_t firstReadTime = lastReadTime;
    LOG_TRACE("Starting reading loop with interval: {}", interval);
    while (keepReading && time(NULL) < firstReadTime + 25)
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


void Modbus::CreateContiniousRegistersSet()
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


void Modbus::ReadCoils()
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


void Modbus::ReadInputs()
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


void Modbus::ReadHoldingRegisters()
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


void Modbus::ReadInputRegisters()
{
    for (RegistersSet& set : continiousRegions_inputRegisters)
    {
        uint16_t* ptr = new uint16_t[set.length];
        returnedValue = modbus_read_input_registers(mb, set.startOffset, set.length, ptr);
        std::memcpy(&inputRegisters[set.startOffset], ptr, set.length * sizeof(uint16_t));
        delete[] ptr;
    }
}


void Modbus::InterpreteRegisters()
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


void Modbus::TranslateRegistersToValue(uint16_t* ptr_registers, ModbusSignal* ptr_signal)
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


void Modbus::TranslateRegistersToValue(uint8_t* ptr_registers, ModbusSignal* ptr_signal)
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


void Modbus::SetSignal(ModbusOrder order)
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


void Modbus::ExecuteOrders()
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
                LOG_WARN("Order with invalid Modbus Region");
            }
        }
        orders.erase(orders.begin());
    }
}


uint16_t* Modbus::TranslateValueToRegisters(ModbusSignal& signal, ModbusValue orderValue)
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
            if (signal.endian == Endian::BIG)
            {
                *ptr = SwapBytesInOrder<uint32_t>((uint16_t*)&orderValue.UINT_32, order32big);
            }
            else
            {
                *ptr = SwapBytesInOrder<uint32_t>((uint16_t*)&orderValue.UINT_32, order32little);
            }
            break;
        }
        case ModbusDataType::INT_32:
        {
            if (signal.endian == Endian::BIG)
            {
                *ptr = SwapBytesInOrder<int32_t>((uint16_t*)&orderValue.INT_32, order32big);
            }
            else
            {
                *ptr = SwapBytesInOrder<int32_t>((uint16_t*)&orderValue.INT_32, order32little);
            }
            break;
        }
        case ModbusDataType::FLOAT:
        {
            uint32_t temp_uint32;
            if (signal.endian == Endian::BIG)
            {
                temp_uint32 = SwapBytesInOrder<uint32_t>((uint16_t*)&orderValue.FLOAT, order32big);
                *ptr = *((float*)&temp_uint32);   
            }
            else
            {
                temp_uint32 = SwapBytesInOrder<uint32_t>((uint16_t*)&orderValue.FLOAT, order32little);
                *ptr = *((float*)&temp_uint32);   
            }
            break;
        }
        case ModbusDataType::UINT_64:
        {
            if (signal.endian == Endian::BIG)
            {
                *ptr = SwapBytesInOrder<uint64_t>((uint16_t*)&orderValue.UINT_64, order64big);
            }
            else
            {
                *ptr = SwapBytesInOrder<uint64_t>((uint16_t*)&orderValue.UINT_64, order64little);
            }
            break;
        }
        case ModbusDataType::INT_64:
        {
            if (signal.endian == Endian::BIG)
            {
                *ptr = SwapBytesInOrder<int64_t>((uint16_t*)&orderValue.INT_64, order64big);
            }
            else
            {
                *ptr = SwapBytesInOrder<int64_t>((uint16_t*)&orderValue.INT_64, order64little);
            }
            break;
        }
        case ModbusDataType::DOUBLE:
        {
            uint64_t temp_uint64;
            if (signal.endian == Endian::BIG)
            {
                temp_uint64 = SwapBytesInOrder<uint64_t>((uint16_t*)&orderValue.DOUBLE, order64big);
                *ptr = *((double*)&temp_uint64);   
            }
            else
            {
                temp_uint64 = SwapBytesInOrder<uint64_t>((uint16_t*)&orderValue.DOUBLE, order64little);
                *ptr = *((double*)&temp_uint64);  
            }
            break;
        }
        default:
        {
            break;
        }
    }
    return ptr;
}


