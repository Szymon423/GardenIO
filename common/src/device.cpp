#include "device.hpp"


Device::~Device()
{

}


Device::Device()
    : deviceType(DeviceType::NON_GENERIC), IP(""), ModbusEnabled(false), deviceName(DeviceTypeToString(DeviceType::NON_GENERIC))
{

}


Device::Device(const DeviceType& dt, const std::string& ip)
    : deviceType(dt), IP(ip), deviceName(DeviceTypeToString(dt)), ModbusEnabled(false)
{

}


Device::Device(const DeviceType& dt, const std::string& ip, bool modbusEnabled, int port, const std::vector<ModbusSignal>& signals)
    : deviceType(dt), IP(ip), ModbusEnabled(modbusEnabled), ModbusPort(port), ModbusSignals(signals), deviceName(DeviceTypeToString(dt))
{

}


Device::Device(const DeviceType& dt, const std::string& ip, const std::string& name, bool modbusEnabled, int port, const std::vector<ModbusSignal>& signals)
    : deviceType(dt), IP(ip), ModbusEnabled(modbusEnabled), ModbusPort(port), ModbusSignals(signals), deviceName(name)
{

}


Device::Device(const Device& dev, const std::string& ip, const std::string& name)
    : deviceType(dev.deviceType), IP(ip), deviceName(name), ModbusEnabled(dev.ModbusEnabled), ModbusPort(dev.ModbusPort), ModbusSignals(dev.ModbusSignals)
{

}


std::vector<ModbusSignal> Device::GetModbusSignals()
{
    return ModbusSignals;
}


std::string Device::PrintDeviceInfo()
{
    std::stringstream ss;
    ss << deviceName << ":\n";
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
            ss << sig.offset;
            if (sig.dataType != ModbusDataType::BOOL)
            {
                ss << " : " << EndianToString(sig.endian) << "\n";
            }
            else
            {
                ss << "\n";
            }
        }
    }
    return ss.str();
}


std::string Device::GetIP()
{
    return IP;
}


int Device::GetModbusPort()
{
    return ModbusPort;
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