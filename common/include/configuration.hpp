#include <string>
#include <nlohmann/json.hpp>

#include "device.hpp"

/*///////////////////////////////////////

GardenIO/configuration/devices.json
GardenIO/configuration/...

*/////////////////////////////////////////

class Configuration
{
private:
    const std::string devicesPath{"GardenIO/configuration/genericDevices.json"};
    std::vector<Device> devices;
    void CheckConfigFileExist();
    void ReadConfigFile();
public:
    Configuration();

    ~Configuration();
    void LoadConfiguration();
    void Print();
};