#include "wind_sensor.hpp"
#include "log.hpp"
#include <thread>
#include <chrono>

WindSensor::WindSensor(std::string deviceName) : serial(deviceName, BaudRate::BR300, DataBits::SEVEN, Parity::NONE, StopBits::ONE), deviceName(deviceName)
{
    serial.SetTimeout(2000);
}

WindSensor::~WindSensor()
{

}

void WindSensor::ReadLoop(int range)
{
    continiueLoop = true;
    serial.Open();
    LOG_TRACE("Start Reading");
    for (int i = 0; ((range == -1) || (i < range)) && continiueLoop; (range == -1) ? i = i : i++)
    {
        while (serial.IsAvaliable() < 14)
        {
            if (!continiueLoop) break;
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        if (!continiueLoop) break;
        rawDataBuff = serial.Read();
        decode();
        LOG_TRACE("Speed: {} m/s, Direction: {} deg", speedValue, directionValue);
    }
    LOG_TRACE("Finished Reading");
    continiueLoop  = false;
}

void WindSensor::decode()
{
    decodedDataBuff = "";
    for (int i = 1; i < rawDataBuff.length() - 1; i++)
    {
        decodedDataBuff += static_cast<char>(rawDataBuff[i]) & (~64);
    }
    if (decodedDataBuff.substr(10, 1) == "0")
    {
        speed = decodedDataBuff.substr(10, 2).insert(1, ".");
    }
    else
    {
        speed = decodedDataBuff.substr(9, 3).insert(2, ".");
    } 
    direction = decodedDataBuff.substr(5, 3);
    speedValue = std::stod(speed);
    directionValue = std::stod(direction);
}

void WindSensor::StopLoop()
{
    continiueLoop = false;
}