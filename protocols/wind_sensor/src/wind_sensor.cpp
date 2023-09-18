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
    serial.Open();
    for (int i = 0; i < range; i++)
    {
        while (serial.IsAvaliable() < 14)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        rawDataBuff = serial.Read();
        decode();
        LOG_TRACE("Speed: {} m/s, Direction: {} deg", speedValue, directionValue);
    }
}

void WindSensor::decode()
{
    decodedDataBuff = "";
    for (int i = 1; i < rawDataBuff.length() - 1; i++)
    {
        decodedDataBuff += static_cast<char>(rawDataBuff[i]) & (~64);
    }
    LOG_TRACE("Raw: {}", rawDataBuff);
    LOG_TRACE("Decoded: {}", decodedDataBuff);
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