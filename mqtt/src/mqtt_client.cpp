#include "mqtt_client.hpp"
#include "log.hpp"
#include <string>



MQTT::MQTT() 
{
    mosq = nullptr;
    valid = 1;
}

MQTT::~MQTT() {}


void MQTT::initialize()
{
    // check if ok to proceed
    if (valid != 1) return;

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
        valid = 0;
		return;
	}

    /* Configure callbacks. This should be done before connecting ideally. */
    mosquitto_connect_callback_set(mosq, MQTT::on_connect);
	mosquitto_publish_callback_set(mosq, MQTT::on_publish);
}

void MQTT::connect(std::string broker_ip, int port, int keepalive)
{
    // check if ok to proceed
    if (valid != 1) return;

    /* Connect to test.mosquitto.org on port 1883, with a keepalive of 60 seconds.
	 * This call makes the socket connection only, it does not complete the MQTT
	 * CONNECT/CONNACK flow, you should use mosquitto_loop_start() or
	 * mosquitto_loop_forever() for processing net traffic. */
    rc = mosquitto_connect(mosq, const_cast<char*>(broker_ip.c_str()), port, keepalive);
	if(rc != MOSQ_ERR_SUCCESS)
    {
		valid = 0;
        mosquitto_destroy(mosq);
        LOG_ERROR("Error: {}", mosquitto_strerror(rc));
		return;
	}
}

void MQTT::run()
{
    // check if ok to proceed
    if (valid != 1) return;
    
    /* Run the network loop in a background thread, this call returns quickly. */
    rc = mosquitto_loop_start(mosq);
	if(rc != MOSQ_ERR_SUCCESS)
    {
		valid = 0;
        mosquitto_destroy(mosq);
		LOG_ERROR("Error: {}", mosquitto_strerror(rc));
		return;
	}
}

void MQTT::shutdown()
{
    mosquitto_lib_cleanup();
}



/* Callback called when the client receives a CONNACK message from the broker. */
void MQTT::on_connect(struct mosquitto *mosq, void *obj, int reason_code)
{
	/* Print out the connection result. mosquitto_connack_string() produces an
	 * appropriate string for MQTT v3.x clients, the equivalent for MQTT v5.0
	 * clients is mosquitto_reason_string().
	 */
    LOG_TRACE("on_connect: {}", mosquitto_connack_string(reason_code));
	if(reason_code != 0){
		/* If the connection fails for any reason, we don't want to keep on
		 * retrying in this example, so disconnect. Without this, the client
		 * will attempt to reconnect. */
		mosquitto_disconnect(mosq);
	}

	/* You may wish to set a flag here to indicate to your application that the
	 * client is now connected. */
}


/* Callback called when the client knows to the best of its abilities that a
 * PUBLISH has been successfully sent. For QoS 0 this means the message has
 * been completely written to the operating system. For QoS 1 this means we
 * have received a PUBACK from the broker. For QoS 2 this means we have
 * received a PUBCOMP from the broker. */
void MQTT::on_publish(struct mosquitto *mosq, void *obj, int mid)
{
	LOG_TRACE("Message with mid {} has been published.", mid);
}


int MQTT::get_temperature(void)
{
	sleep(1); /* Prevent a storm of messages - this pretend sensor works at 1Hz */
	return random()%100;
}

/* This function pretends to read some data from a sensor and publish it.*/
void MQTT::publish_sensor_data()
{
	char payload[20];
	int temp;
	int rc;

	/* Get our pretend data */
	temp = get_temperature();
	/* Print it to a string for easy human reading - payload format is highly
	 * application dependent. */
	snprintf(payload, sizeof(payload), "%d", temp);

	/* Publish the message
	 * mosq - our client instance
	 * *mid = NULL - we don't want to know what the message id for this message is
	 * topic = "example/temperature" - the topic on which this message will be published
	 * payloadlen = strlen(payload) - the length of our payload in bytes
	 * payload - the actual payload
	 * qos = 2 - publish with QoS 2 for this example
	 * retain = false - do not use the retained message feature for this message
	 */
	rc = mosquitto_publish(mosq, NULL, "test_topic", strlen(payload), payload, 2, false);
	if(rc != MOSQ_ERR_SUCCESS)
    {
	    LOG_ERROR("Error publishing: {}", mosquitto_strerror(rc));
	}
}