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
	mosquitto_disconnect_callback_set(mosq, MQTT::on_disconnect);
	mosquitto_publish_callback_set(mosq, MQTT::on_publish);
	mosquitto_subscribe_callback_set(mosq, MQTT::on_subscribe);
	mosquitto_unsubscribe_callback_set(mosq, MQTT::on_unsubscribe);
	mosquitto_message_callback_set(mosq, MQTT::on_message);
	mosquitto_log_callback_set(mosq, MQTT::on_log);

}

void MQTT::connect(std::string broker_ip, int port, int keepalive, reconnect type)
{
    // check if ok to proceed
    if (valid != 1) return;

    /* Connect to test.mosquitto.org on port 1883, with a keepalive of 60 seconds.
	 * This call makes the socket connection only, it does not complete the MQTT
	 * CONNECT/CONNACK flow, you should use mosquitto_loop_start() or
	 * mosquitto_loop_forever() for processing net traffic. */

	while (1)
	{
    	LOG_TRACE("Connecting to broker at: {}:{}", broker_ip, port);
		rc = mosquitto_connect(mosq, broker_ip.c_str(), port, keepalive);
		if(rc != MOSQ_ERR_SUCCESS)
		{		
			valid = 0;
			mosquitto_destroy(mosq);
			LOG_ERROR("Error: {}", mosquitto_strerror(rc));
			// sleep for 1s before next attempt
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
		else
		{
			break;
		}
	} 

	if (type == reconnect::RECONNECT)
	{
		rc = mosquitto_reconnect(mosq);
		LOG_TRACE("Reconecting enabled");
	}
	else
	{
		LOG_TRACE("Reconecting disabled");
	}
}

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

void MQTT::subscribe(std::string topic)
{
	// check if ok to proceed
    if (valid != 1) return;

	/* Making subscriptions in the on_connect() callback means that if the
	 * connection drops and is automatically resumed by the client, then the
	 * subscriptions will be recreated when the client reconnects. */
	rc = mosquitto_subscribe(mosq, NULL, const_cast<char*>(topic.c_str()), 1);
	if (rc != MOSQ_ERR_SUCCESS)
	{
		LOG_ERROR("Error subscribing: {}", mosquitto_strerror(rc));
		/* We might as well disconnect if we were unable to subscribe */
		mosquitto_disconnect(mosq);
	}
}

void MQTT::on_subscribe(struct mosquitto *mosq, void *obj, int mid, int qos_count, const int *granted_qos)
{
	int i;
	bool have_subscription = false;

	/* In this example we only subscribe to a single topic at once, but a
	 * SUBSCRIBE can contain many topics at once, so this is one way to check
	 * them all. */
	for (i = 0; i < qos_count; i++)
	{
		LOG_TRACE("on_subscribe: {} granted qos = {}", i, granted_qos[i]);
		if (granted_qos[i] <= 2)
		{
			have_subscription = true;
		}
	}
	if (have_subscription == false)
	{
		/* The broker rejected all of our subscriptions, we know we only sent
		 * the one SUBSCRIBE, so there is no point remaining connected. */
		LOG_ERROR("Error: All subscriptions rejected.");
		mosquitto_disconnect(mosq);
	}
}

void MQTT::on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
{
	/* This blindly prints the payload, but the payload can be anything so take care. */
	LOG_TRACE("on_message: {} at topic: {} with qos: {}", (char *)msg->payload, msg->topic, msg->qos);
}

void MQTT::publish(std::string topic, std::string payload)
{
	const char *_payload = payload.c_str();

	/* Publish the message
	 * mosq - our client instance
	 * *mid = NULL - we don't want to know what the message id for this message is
	 * topic = "example/temperature" - the topic on which this message will be published
	 * payloadlen = strlen(payload) - the length of our payload in bytess
	 * payload - the actual payload
	 * qos = 2 - publish with QoS 2 for this example
	 * retain = false - do not use the retained message feature for this message
	 */
	rc = mosquitto_publish(mosq, NULL, const_cast<char*>(topic.c_str()), strlen(_payload), _payload, 2, false);
	if (rc != MOSQ_ERR_SUCCESS)
    {
	    LOG_ERROR("Error publishing: {}", mosquitto_strerror(rc));
		valid = 0;
	}
}

void MQTT::on_publish(struct mosquitto *mosq, void *obj, int mid)
{
	/* Callback called when the client knows to the best of its abilities that a
	* PUBLISH has been successfully sent. For QoS 0 this means the message has
	* been completely written to the operating system. For QoS 1 this means we
	* have received a PUBACK from the broker. For QoS 2 this means we have
	* received a PUBCOMP from the broker. */
	LOG_TRACE("on publish: published message with mid {}.", mid);
}

void MQTT::on_unsubscribe(struct mosquitto *mosq, void *obj, int mid)
{
	LOG_TRACE("on unsubscribe: unsubscribed to topic.");
}

void MQTT::on_log(struct mosquitto *mosq, void *obj, int level, const char *str)
{
	// LOG_TRACE("on log: {}.", str);
}

void MQTT::on_disconnect(struct mosquitto *mosq, void *obj, int reason_code)
{
	LOG_TRACE("on disconnect: {}", mosquitto_connack_string(reason_code));
}