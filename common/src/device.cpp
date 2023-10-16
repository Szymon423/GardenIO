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