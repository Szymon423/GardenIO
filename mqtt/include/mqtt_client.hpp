#pragma once

#include <mosquitto.h>

void on_connect(struct mosquitto *mosq, void *obj, int reason_code);

void on_publish(struct mosquitto *mosq, void *obj, int mid);

int get_temperature(void);

void publish_sensor_data(struct mosquitto *mosq);