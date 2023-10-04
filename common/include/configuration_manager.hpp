#include <fstream>
#include <string>


///
/// So the system is going to work like this:
///     > Server is listening on special MQTT topic named: Introduce
///     > New nodes are supposed to send special message on this topic with their name, 
///       MAC addres and node type information
///     > Server then responds to them with set of their signals ID's
///     > If necessary server invouqe reloadConfiguration() function to update congfig
///


class ConfigManager
{
private:
    std::string configFilePath;
public:
    ConfigManager(std::string configFilePath);
    ConfigManager() = default;
    ~ConfigManager() = default;

    void reloadConfiguration();
};