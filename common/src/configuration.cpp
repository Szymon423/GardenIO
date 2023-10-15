#include "configuration.hpp"

#include <iostream>
#include <filesystem>

#include "log.hpp"

Configuration::Configuration()
{

}


Configuration::~Configuration()
{

}


void Configuration::CheckConfigFileExist()
{
    if (std::filesystem::exists(devicesPath.c_str()))
    {
        LOG_TRACE("Config file: GardenIO/configuration/devices.json exist");
    }
    else
    {
        LOG_WARN("File: GardenIO/configuration/devices.json not found");
    }
}


void Configuration::LoadConfiguration()
{

}


void Configuration::ReloadConfiguration()
{
    
}