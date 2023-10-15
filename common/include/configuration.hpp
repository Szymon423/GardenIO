#include <string>

#include "modbus_definitions.hpp"

/*///////////////////////////////////////

GardenIO/configuration/devices.json
GardenIO/configuration/...

*/////////////////////////////////////////

class Configuration
{
private:
    const std::string devicesPath = "configuration/devices.json";
    
    void CheckConfigFileExist();
public:
    Configuration();
    ~Configuration();
    void LoadConfiguration();
    void ReloadConfiguration();
};