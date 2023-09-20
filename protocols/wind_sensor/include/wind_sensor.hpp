#include "serial_lib.hpp"
#include <string>



class WindSensor
{
private:
    Serial serial;
    std::string deviceName;
    std::string rawDataBuff;
    std::string decodedDataBuff;
    std::string speed;
    std::string direction;
    double speedValue;
    double directionValue;
    void decode();
    bool continiueLoop;
public:
    WindSensor(std::string deviceName);
    ~WindSensor();
    void ReadLoop(int range);
    void StopLoop();
};
