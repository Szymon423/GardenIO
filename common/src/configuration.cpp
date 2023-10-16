#include "configuration.hpp"
#include "log.hpp"
#include "modbus_definitions.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>


Configuration::Configuration()
{

}


Configuration::~Configuration()
{

}


void Configuration::CheckConfigFileExist()
{
    if (std::filesystem::exists(devicesPath.c_str()))
    {
        LOG_TRACE("Config file: ./GardenIO/configuration/devices.json exist");
    }
    else
    {
        LOG_WARN("File: ./GardenIO/configuration/devices.json not found");
    }
}


void Configuration::ReadConfigFile()
{
    using json = nlohmann::json;
    std::ifstream f(devicesPath);
    json data = json::parse(f);
    for (auto& dev : data["genericDevices"])
    {
        
        DeviceType deviceType = StringToDeviceType(dev["DeviceType"].get<std::string>());
        std::string ip = dev["IP"].get<std::string>();
        bool modbusEnabled = dev["ModbusEnabled"].get<bool>();
        int port = dev["ModbusPort"].get<int>();
        std::vector<ModbusSignal> modbusSignals;
        for (auto& sig : dev["ModbusSignals"])
        {
            int offset = sig["offset"].get<int>();
            ModbusDataType dataType = StringToModbusDataType(sig["ModbusDataType"].get<std::string>());
            Endian endian = StringToEndian(sig["Endian"].get<std::string>());
            ModbusRegion region = StringToModbusRegion(sig["ModbusRegion"].get<std::string>());
            modbusSignals.push_back(ModbusSignal(endian, dataType, region, offset));
        }
        devices.push_back(Device(deviceType, ip, modbusEnabled, port, modbusSignals));
    }
}


void Configuration::LoadConfiguration()
{
    devices.clear();
    ReadConfigFile()
}


DeviceType StringToDeviceType(std::string deviceType)
{
    if (deviceType == "NODE_1") return DeviceType::NODE_1;
    if (deviceType == "NODE_2") return DeviceType::NODE_2;
    return DeviceType::NON_GENERIC;
}