#include "device.hpp"


Device::~Device()
{

}


Device::Device()
{

}


Device::Device(DeviceType dt, std::string ip)
    : deviceType(dt), IP(ip)
{

}

Device::Device(DeviceType dt, std::string ip, bool modbusEnabled, int port, std::vector<ModbusSignal> signals)
    : deviceType(dt), IP(ip), ModbusEnabled(modbusEnabled), ModbusPort(port), ModbusSignals(signals)
{

}


std::string Device::PrintDeviceInfo()
{
    std::stringstream ss;
    ss << DeviceTypeToString(deviceType) + "\t";
    ss << "IP: " << IP;
    if (ModbusEnabled)
    {
        ss << ":" << ModbusPort << "\n";
        ss << "Modbus Signals: \n";
        for (const auto& sig : ModbusSignals)
        {
            ss << ModbusRegionToString(sig.region) << " : ";
            ss << ModbusDataTypeToString(sig.dataType) << " : ";
            ss << sig.offset << " : ";
            ss << EndianToString(sig.endian) << "\n";
        }
    }
    return ss.str();
}


DeviceType StringToDeviceType(std::string deviceType)
{
    if (deviceType == "NODE_1") return DeviceType::NODE_1;
    if (deviceType == "NODE_2") return DeviceType::NODE_2;
    return DeviceType::NON_GENERIC;
}


std::string DeviceTypeToString(DeviceType deviceType)
{
    if (deviceType == DeviceType::NODE_1) return "NODE_1";
    if (deviceType == DeviceType::NODE_2) return "NODE_2";
    return "NON_GENERIC";
}