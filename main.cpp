#include "sqlite_wrapper.hpp"
#include "signals_definitions.hpp"
#include <iostream>
#include <cstring>
#include "log_manager.hpp"
#include "log.hpp"
#include "mqtt_client.hpp"
#include <mosquitto.h>

int main()
{
    logger::manager LOG;
    LOG.Initialize("/home/szymon/GardenIO/GardenIO.log");


    struct mosquitto *mosq;
	int rc;

	/* Required before calling other mosquitto functions */
	mosquitto_lib_init();

	/* Create a new client instance.
	 * id = NULL -> ask the broker to generate a client id for us
	 * clean session = true -> the broker should remove old sessions when we connect
	 * obj = NULL -> we aren't passing any of our private data for callbacks
	 */
	mosq = mosquitto_new(NULL, true, NULL);
	if(mosq == NULL)
    {
        LOG_ERROR("Error: Out of memory.");
		return 1;
	}

	/* Configure callbacks. This should be done before connecting ideally. */
	mosquitto_connect_callback_set(mosq, on_connect);
	mosquitto_publish_callback_set(mosq, on_publish);

	/* Connect to test.mosquitto.org on port 1883, with a keepalive of 60 seconds.
	 * This call makes the socket connection only, it does not complete the MQTT
	 * CONNECT/CONNACK flow, you should use mosquitto_loop_start() or
	 * mosquitto_loop_forever() for processing net traffic. */
	rc = mosquitto_connect(mosq, "localhost", 1883, 60);
	if(rc != MOSQ_ERR_SUCCESS)
    {
		mosquitto_destroy(mosq);
        LOG_ERROR("Error: {}", mosquitto_strerror(rc));
		return 1;
	}

	/* Run the network loop in a background thread, this call returns quickly. */
	rc = mosquitto_loop_start(mosq);
	if(rc != MOSQ_ERR_SUCCESS)
    {
		mosquitto_destroy(mosq);
		LOG_ERROR("Error: {}", mosquitto_strerror(rc));
		return 1;
	}

	/* At this point the client is connected to the network socket, but may not
	 * have completed CONNECT/CONNACK.
	 * It is fairly safe to start queuing messages at this point, but if you
	 * want to be really sure you should wait until after a successful call to
	 * the connect callback.
	 * In this case we know it is 1 second before we start publishing.
	 */
	while(1){
		publish_sensor_data(mosq);
	}

	mosquitto_lib_cleanup();







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