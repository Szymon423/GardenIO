#include "configuration.hpp"
#include "log.hpp"
#include "modbus_definitions.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>


Configuration::Configuration()
{
    CheckConfigFileExist();
}


Configuration::~Configuration()
{

}


void Configuration::CheckConfigFileExist()
{
    namespace fs = std::filesystem;
    fs::path p = fs::current_path() / devicesPath;
    if (fs::exists(p))
    {
        LOG_TRACE("Config file: {} exist", std::string(p.u8string()));
    }
    else
    {
        LOG_WARN("File: {} not found", std::string(p.u8string()));
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
    ReadConfigFile();
}


void Configuration::Print()
{
    for (auto& dev : devices)
    {
        std::cout << dev.PrintDeviceInfo() << std::endl;
    }
}