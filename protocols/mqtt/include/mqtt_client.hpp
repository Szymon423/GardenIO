#pragma once

#include <mosquitto.h>
#include <string>


enum reconnect
{
    NO_RECONNECT,
    RECONNECT
};

class MQTT
{
public:

    struct mosquitto *mosq;
    int rc;
    int valid;

    MQTT();
    ~MQTT();
    void initialize();
    void connect(std::string broker_ip, int port, int keepalive, reconnect type);
    void run();
    void shutdown();
    void subscribe(std::string topic);
    void publish(std::string topic, std::string payload);
    static void on_connect(struct mosquitto *mosq, void *obj, int reason_code);
    static void on_disconnect(struct mosquitto *mosq, void *obj, int reason_code);
    static void on_publish(struct mosquitto *mosq, void *obj, int mid);
    static void on_subscribe(struct mosquitto *mosq, void *obj, int mid, int qos_count, const int *granted_qos);
    static void on_unsubscribe(struct mosquitto *mosq, void *obj, int mid);
    static void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg);
    static void on_log(struct mosquitto *mosq, void *obj, int level, const char *str);
};