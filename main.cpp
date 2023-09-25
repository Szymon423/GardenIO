#include <iostream>
#include <string>
#include <cstring>
#include <thread>
#include <chrono>

#include "sqlite_wrapper.hpp"
#include "signals_definitions.hpp"
#include "log_manager.hpp"
#include "log.hpp"
#include "mqtt_client.hpp"
#include "wind_sensor.hpp"

int main()
{
    logger::manager LOG;
    LOG.Initialize("/home/szymon/GardenIO/GardenIO.log");

    signals::analog_signal_t my_analog(time(NULL), 1.234, "m/s", 1, "0", signals::actionType::info, 0);

    std::string composedMsg = signals::composeMessage(my_analog);
    LOG_TRACE("oryginal signal: {}", std::string(my_analog));

    signals::analog_signal_t new_analog;

    signals::decomposeMessage(composedMsg, new_analog);
    LOG_TRACE("new signal: {}", std::string(new_analog));










    // WindSensor ws("/dev/ttyUSB0");  
    // std::thread wsThread(&WindSensor::ReadLoop, &ws, -1);
    // LOG_TRACE("Started Wind Sensor thread");

    // std::this_thread::sleep_for(std::chrono::seconds(20));


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