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
    std::string deviceName;
    bool ModbusEnabled;
    int ModbusPort;
    std::vector<ModbusSignal> ModbusSignals;

public:
    ~Device();
    Device();
    Device(const DeviceType& dt, const std::string& ip);
    Device(const DeviceType& dt, const std::string& ip, bool modbusEnabled, int port, const std::vector<ModbusSignal>& signals);
    Device(const DeviceType& dt, const std::string& ip, const std::string& name, bool modbusEnabled, int port, const std::vector<ModbusSignal>& signals);
    Device(const Device& dev, const std::string& ip, const std::string& name);
    std::vector<ModbusSignal> GetModbusSignals();
    std::string PrintDeviceInfo();
    std::string GetIP();
    int GetModbusPort();
};


DeviceType StringToDeviceType(std::string deviceType);
std::string DeviceTypeToString(DeviceType deviceType);