#include <string>
#include <map>
#include <nlohmann/json.hpp>

#include "device.hpp"

/*///////////////////////////////////////

GardenIO/configuration/devices.json
GardenIO/configuration/...

*/////////////////////////////////////////

class Configuration
{
private:
    const std::string genericDevicesPath{"GardenIO/configuration/genericDevices.json"};
    const std::string devicesPath{"GardenIO/configuration/devices.json"};
    std::vector<Device> devices;
    std::vector<Device> genericDevices;
    std::map<DeviceType, int> map;
    void CheckFileExist(const std::string& path);
    void ReadGenericConfigFile();
    void ReadConfigFile();
    void LoadGenericConfiguration();
    
public:
    Configuration();
    ~Configuration();
    void LoadConfiguration();
    void Print();
};