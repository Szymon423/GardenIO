#include "modbus_wrapper.hpp"
#include "modbus_definitions.hpp"
#include <cstdint>
#include <cstring>

void Modbus::SetConnectionParams(std::string ip, int port)
{
    ip = ip;
    port = port;
}

void Modbus::SetConnectionInterval(time_t interval)
{
    interval = interval;
}

void Modbus::SetSignalsDefinitions(std::vector<ModbusSignal> signalsDefinitions)
{
    modbusSignals = signalsDefinitions;
}

void Modbus::RunInLoop()
{
    keepReading = true;
    lastReadTime = time(NULL);
    while (keepReading)
    {
        if (time(NULL) > lastReadTime + interval)
        {
            ReadCoils();
            ReadInputs();
            ReadHoldingRegisters();
            ReadInputRegisters();
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
    RegistersSet set;
    
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

    for (int i = 0; i < _coils.size(); i++)
    {
        if (i == 0)
        {
            set.startOffset = _coils.at(i).offset;
            set.length = DataTypeLength(_coils.at(i).dataType);
        }
        else if (set.length > set.startOffset + DataTypeLength(_coils.at(i).dataType))
        {
            continiousRegions_coils.push_back(set);
        }
        else
        {
            set.length += DataTypeLength(_coils.at(i).dataType);
        }
    }

    for (int i = 0; i < _inputs.size(); i++)
    {
        if (i == 0)
        {
            set.startOffset = _inputs.at(i).offset;
            set.length = DataTypeLength(_inputs.at(i).dataType);
        }
        else if (set.length > set.startOffset + DataTypeLength(_inputs.at(i).dataType))
        {
            continiousRegions_inputs.push_back(set);
        }
        else
        {
            set.length += DataTypeLength(_inputs.at(i).dataType);
        }
    }

    for (int i = 0; i < _holdingRegisters.size(); i++)
    {
        if (i == 0)
        {
            set.startOffset = _holdingRegisters.at(i).offset;
            set.length = DataTypeLength(_holdingRegisters.at(i).dataType);
        }
        else if (set.length > set.startOffset + DataTypeLength(_holdingRegisters.at(i).dataType))
        {
            continiousRegions_holdingRegisters.push_back(set);
        }
        else
        {
            set.length += DataTypeLength(_holdingRegisters.at(i).dataType);
        }
    }

    for (int i = 0; i < _inputRegisters.size(); i++)
    {
        if (i == 0)
        {
            set.startOffset = _inputRegisters.at(i).offset;
            set.length = DataTypeLength(_inputRegisters.at(i).dataType);
        }
        else if (set.length > set.startOffset + DataTypeLength(_inputRegisters.at(i).dataType))
        {
            continiousRegions_inputRegisters.push_back(set);
        }
        else
        {
            set.length += DataTypeLength(_inputRegisters.at(i).dataType);
        }
    }
}

void Modbus::ReadCoils()
{
    for (RegistersSet& set : continiousRegions_coils)
    {
        uint8_t* ptr = new uint8_t[set.length];
        returnedValue = modbus_read_bits(mb, set.startOffset, set.length, ptr);
        std::memcpy(&coils[set.startOffset], ptr, sizeof(uint8_t*) * set.length);
        delete[] ptr;
    }
}

void Modbus::ReadInputs()
{
    for (RegistersSet& set : continiousRegions_inputs)
    {
        uint8_t* ptr = new uint8_t[set.length];
        returnedValue = modbus_read_input_bits(mb, set.startOffset, set.length, ptr);
        std::memcpy(&inputs[set.startOffset], ptr, sizeof(uint8_t*) * set.length);
        delete[] ptr;
    }
}

void Modbus::ReadHoldingRegisters()
{
    for (RegistersSet& set : continiousRegions_holdingRegisters)
    {
        uint16_t* ptr = new uint16_t[set.length];
        returnedValue = modbus_read_registers(mb, set.startOffset, set.length, ptr);
        std::memcpy(&holdingRegisters[set.startOffset], ptr, sizeof(uint16_t*) * set.length);
        delete[] ptr;
    }
}

void Modbus::ReadInputRegisters()
{
    for (RegistersSet& set : continiousRegions_inputRegisters)
    {
        uint16_t* ptr = new uint16_t[set.length];
        returnedValue = modbus_read_input_registers(mb, set.startOffset, set.length, ptr);
        std::memcpy(&inputRegisters[set.startOffset], ptr, sizeof(uint16_t*) * set.length);
        delete[] ptr;
    }
}

void Modbus::InterpreteRegisters()
{
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
        
    }
}

void Modbus::TranslateRegistersToValue(uint16_t* ptr_registers, ModbusSignal* ptr_signal)
{
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
            uint32_t* ptr_proper_type = nullptr;
            if (ptr_signal->endian == Endian::LITTLE)
            {
                ptr_signal->value.UINT_32 = *((uint32_t*)ptr_registers);   
            }
            else
            {
                uint32_t high = *((uint32_t*)ptr_registers) && 0xff00;
                uint32_t low = *((uint32_t*)ptr_registers) && 0x00ff;
                ptr_signal->value.UINT_32 = high >> 16 | low << 16;
            }
            break;
        }
        case ModbusDataType::INT_32:
        {
            int32_t* ptr_proper_type = nullptr;
            if (ptr_signal->endian == Endian::LITTLE)
            {
                ptr_signal->value.INT_32 = *((int32_t*)ptr_registers);   
            }
            else
            {
                int32_t high = *((int32_t*)ptr_registers) && 0xff00;
                int32_t low = *((int32_t*)ptr_registers) && 0x00ff;
                ptr_signal->value.UINT_32 = high >> 16 | low << 16;
            }
            break;
        }
        case ModbusDataType::FLOAT:
        {
            uint32_t* ptr_uint32 = nullptr;
            float* ptr_proper_type = nullptr;
            if (ptr_signal->endian == Endian::LITTLE)
            {
                ptr_uint32 = (uint32_t*)ptr_registers;
                ptr_signal->value.FLOAT = *((float*)ptr_uint32);   
            }
            else
            {
                uint32_t high = *((uint32_t*)ptr_registers) && 0xff00;
                uint32_t low = *((uint32_t*)ptr_registers) && 0x00ff;
                uint32_t temp = high >> 16 | low << 16;
                ptr_signal->value.FLOAT = *((float*)&temp);
            }
            break;
        }
        case ModbusDataType::UINT_64:
        {
            uint64_t* ptr_proper_type = nullptr;
            if (ptr_signal->endian == Endian::LITTLE)
            {
                ptr_signal->value.UINT_64 = *((uint64_t*)ptr_registers);   
            }
            else
            {
                uint64_t high = *((uint64_t*)ptr_registers) && 0xff000000;
                uint64_t mid_high = *((uint64_t*)ptr_registers) && 0x00ff0000;
                uint64_t mid_low = *((uint64_t*)ptr_registers) && 0x0000ff00;
                uint64_t low = *((uint64_t*)ptr_registers) && 0x000000ff;
                ptr_signal->value.UINT_64 = high >> 32 | mid_high >> 16 | mid_low << 16 | low << 32;
            }
            break;
        }
        case ModbusDataType::INT_64:
        {
            int64_t* ptr_proper_type = nullptr;
            if (ptr_signal->endian == Endian::LITTLE)
            {
                ptr_signal->value.INT_64 = *((int64_t*)ptr_registers);   
            }
            else
            {
                int64_t high = *((int64_t*)ptr_registers) && 0xff000000;
                int64_t mid_high = *((int64_t*)ptr_registers) && 0x00ff0000;
                int64_t mid_low = *((int64_t*)ptr_registers) && 0x0000ff00;
                int64_t low = *((int64_t*)ptr_registers) && 0x000000ff;
                ptr_signal->value.INT_64 = high >> 32 | mid_high >> 16 | mid_low << 16 | low << 32;
            }
            break;
        }
        case ModbusDataType::DOUBLE:
        {
            uint64_t* ptr_uint64 = nullptr;
            double* ptr_proper_type = nullptr;
            if (ptr_signal->endian == Endian::LITTLE)
            {
                ptr_uint64 = (uint64_t*)ptr_registers;
                ptr_signal->value.DOUBLE = *((double*)ptr_uint64);   
            }
            else
            {
                uint64_t high = *((uint64_t*)ptr_registers) && 0xff000000;
                uint64_t mid_high = *((uint64_t*)ptr_registers) && 0x00ff0000;
                uint64_t mid_low = *((uint64_t*)ptr_registers) && 0x0000ff00;
                uint64_t low = *((uint64_t*)ptr_registers) && 0x000000ff;
                uint64_t temp = high >> 32 | mid_high >> 16 | mid_low << 16 | low << 32;
                ptr_signal->value.DOUBLE = *((double*)&temp);
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