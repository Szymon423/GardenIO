#include "serial_lib.hpp"
#include "sqlite_wrapper.hpp"
#include <string>



class WindSensor
{
private:
    sqlite_wrapper* ptr_db;
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
    WindSensor(std::string deviceName, sqlite_wrapper* ptr_db);
    ~WindSensor();
    void ReadLoop(int range);
    void StopLoop();
};
