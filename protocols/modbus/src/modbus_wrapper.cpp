#include "modbus_wrapper.hpp"
#include "modbus_definitions.hpp"


void Modbus::SetConnectionParams(std::string ip, int port)
{
    this->ip = ip;
    this->port = port;
}

void Modbus::SetConnectionInterval(time_t interval)
{
    this->interval = interval;
}

void Modbus::SetSignalsDefinitions(std::vector<ModbusSignal> signalsDefinitions)
{
    this->modbusSignals = signalsDefinitions;
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
        returnedValue = modbus_read_bits(mb, set.startOffset, set.length, coils[set.startOffset]);
    }
}

void Modbus::ReadInputs()
{
    for (RegistersSet& set : continiousRegions_inputs)
    {
        returnedValue = modbus_read_input_bits(mb, set.startOffset, set.length, inputs[set.startOffset]);
    }
}

void Modbus::ReadHoldingRegisters()
{
    for (RegistersSet& set : continiousRegions_holdingRegisters)
    {
        returnedValue = modbus_read_registers(mb, set.startOffset, set.length, holdingRegisters[set.startOffset]);
    }
}

void Modbus::ReadInputRegisters()
{
    for (RegistersSet& set : continiousRegions_inputRegisters)
    {
        returnedValue = modbus_read_input_registers(mb, set.startOffset, set.length, inputRegisters[set.startOffset]);
    }
}