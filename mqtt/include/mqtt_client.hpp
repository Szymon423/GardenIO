#pragma once

#include <mosquitto.h>
#include <string>

class MQTT
{
public:

    struct mosquitto *mosq;
    int rc;
    int valid;



    MQTT();
    ~MQTT();
    void initialize();
    void connect(std::string broker_ip, int port, int keepalive);
    void run();
    void shutdown();
    static void on_connect(struct mosquitto *mosq, void *obj, int reason_code);
    static void on_publish(struct mosquitto *mosq, void *obj, int mid);
    void publish_sensor_data();
    int get_temperature(void);
};