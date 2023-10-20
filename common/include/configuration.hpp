#include <string>
#include <map>
#include <vector>

#include <nlohmann/json.hpp>

#include "device.hpp"

/*///////////////////////////////////////

GardenIO/configuration/devices.json
GardenIO/configuration/genericDevices.json

*/////////////////////////////////////////

class Configuration
{
private:
    const std::string genericDevicesPath{"GardenIO/configuration/genericDevices.json"};
    const std::string devicesPath{"GardenIO/configuration/devices.json"};
    std::vector<Device> genericDevices;
    std::map<DeviceType, int> map;
    void CheckFileExist(const std::string& path);
    void ReadGenericConfigFile();
    void ReadConfigFile();
    void LoadGenericConfiguration();
    
public:
    std::vector<Device> devices;
    Configuration();
    ~Configuration();
    void LoadConfiguration();
    std::vector<Device> GetDevices();
    void Print();
};