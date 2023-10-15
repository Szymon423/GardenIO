#include <string>
#include <vector>
#include "modbus_definitions.hpp"


enum class DeviceType
{
    NODE_1,
    NODE_2,
    NON_GENERIC
};


class Device
{
private:
    DeviceType deviceType;
    std::string IP;
    bool ModbusEnabled;
    int ModbusPort;
    std::vector<ModbusSignal> modbusSignals;

public:
    ~Device();
    Device();
    Device(DeviceType dt, std::string ip);
};