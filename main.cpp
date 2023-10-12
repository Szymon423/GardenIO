#include <iostream>
#include <string>
#include <cstring>
#include <thread>
#include <chrono>

#include "sqlite_wrapper.hpp"
#include "modbus_wrapper.hpp"
#include "modbus_definitions.hpp"
#include "signals_definitions.hpp"
#include "log_manager.hpp"
#include "log.hpp"
#include "mqtt_client.hpp"
#include "wind_sensor.hpp"

int main()
{
    logger::manager LOG;
    LOG.Initialize("./GardenIO.log");
    
    sqlite_wrapper db("./database/test.db");
    int ret = db.create_tables();

    Modbus mb;
    mb.SetConnectionParams("192.168.0.172", 502);
    mb.SetConnectionInterval(3);
    
    std::vector<ModbusSignal> vect;
    vect.push_back(ModbusSignal(Endian::BIG, ModbusDataType::UINT_16, ModbusRegion::INPUT_REGISTERS, 0));
    vect.push_back(ModbusSignal(Endian::BIG, ModbusDataType::INT_16, ModbusRegion::INPUT_REGISTERS, 1));
    vect.push_back(ModbusSignal(Endian::BIG, ModbusDataType::UINT_32, ModbusRegion::INPUT_REGISTERS, 2));
    vect.push_back(ModbusSignal(Endian::LITTLE, ModbusDataType::UINT_32, ModbusRegion::INPUT_REGISTERS, 4));
    vect.push_back(ModbusSignal(Endian::BIG, ModbusDataType::INT_32, ModbusRegion::INPUT_REGISTERS, 6));
    vect.push_back(ModbusSignal(Endian::LITTLE, ModbusDataType::INT_32, ModbusRegion::INPUT_REGISTERS, 8));
    vect.push_back(ModbusSignal(Endian::BIG, ModbusDataType::UINT_64, ModbusRegion::INPUT_REGISTERS, 10));
    vect.push_back(ModbusSignal(Endian::LITTLE, ModbusDataType::UINT_64, ModbusRegion::INPUT_REGISTERS, 14));
    vect.push_back(ModbusSignal(Endian::BIG, ModbusDataType::INT_64, ModbusRegion::INPUT_REGISTERS, 18));
    vect.push_back(ModbusSignal(Endian::LITTLE, ModbusDataType::INT_64, ModbusRegion::INPUT_REGISTERS, 22));
    vect.push_back(ModbusSignal(Endian::BIG, ModbusDataType::FLOAT, ModbusRegion::INPUT_REGISTERS, 26));
    vect.push_back(ModbusSignal(Endian::LITTLE, ModbusDataType::FLOAT, ModbusRegion::INPUT_REGISTERS, 28));
    vect.push_back(ModbusSignal(Endian::BIG, ModbusDataType::DOUBLE, ModbusRegion::INPUT_REGISTERS, 30));
    vect.push_back(ModbusSignal(Endian::LITTLE, ModbusDataType::DOUBLE, ModbusRegion::INPUT_REGISTERS, 34));
    mb.SetSignalsDefinitions(vect);
    mb.RunInLoop();

    // WindSensor ws("/dev/ttyUSB0", &db);  
    // std::thread wsThread(&WindSensor::ReadLoop, &ws, -1);
    // LOG_TRACE("Started Wind Sensor thread");

    // std::this_thread::sleep_for(std::chrono::seconds(20));












    // signals::Signal my_analog(signals::signalType::Analog, time(NULL), 1.234, "m/s", 1, "0", signals::actionType::info, 0);

    // std::string composedMsg = signals::composeMessage(my_analog);
    // LOG_TRACE("old: {}", std::string(my_analog));
    
    // ret = db.insert(my_analog);

    // signals::Signal new_analog = signals::decomposeMessage(composedMsg);
    // LOG_TRACE("new: {}", std::string(new_analog));

    // ret = db.insert(new_analog);

    // LOG_TRACE("Manually stop Wind Sensor");
    // ws.StopLoop();
    // wsThread.join();
    // LOG_TRACE("Wind Sensor thread finished");

    // MQTT mqtt;
    // mqtt.initialize();
    // mqtt.connect("localhost", 1883, 60, reconnect::RECONNECT);
    // mqtt.run();
    // mqtt.subscribe("test_topic");


	// /* At this point the client is connected to the network socket, but may not
	//  * have completed CONNECT/CONNACK.
	//  * It is fairly safe to start queuing messages at this point, but if you
	//  * want to be really sure you should wait until after a successful call to
	//  * the connect callback.
	//  * In this case we know it is 1 second before we start publishing.
	//  */
	// for (int i = 0; i < 10; i++)
    // {
	// 	mqtt.publish("test_topic", "dupa");
	// }

    // // sleep for 5s
    // std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    // mqtt.shutdown();
    
    // signals::analog_signal_t my_struct;
    // my_struct.time = 123456;
    // my_struct.value = 9.9;
    // strcpy(my_struct.unit, "*C");
    // my_struct.valid = 1;
    // strcpy(my_struct.source, "test");
    // my_struct.type = signals::actionType::info;


    // LOG_TRACE(std::string(my_struct));
    // // std::cout << std::string(my_struct) << std::endl;

    // const char *db_path {"/home/szymon/GardenIO/database/test.db"};
    // sqlite_wrapper db(db_path);
    
    // int ret;
    // ret = db.create_tables();
    // ret = db.insert(my_struct);

    LOG.Shutdown();
}