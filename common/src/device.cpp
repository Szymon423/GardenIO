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