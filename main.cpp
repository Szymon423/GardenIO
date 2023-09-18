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

    
    WindSensor ws("/dev/ttyUSB0");
    ws.ReadLoop(10);















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
    // my_struct.type = signals::signal_type::info;


    // LOG_TRACE(std::string(my_struct));
    // // std::cout << std::string(my_struct) << std::endl;

    // const char *db_path {"/home/szymon/GardenIO/database/test.db"};
    // sqlite_wrapper db(db_path);
    
    // int ret;
    // ret = db.create_tables();
    // ret = db.insert(my_struct);

    LOG.Shutdown();
}