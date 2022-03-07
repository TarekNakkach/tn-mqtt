#ifndef MQTT_SUBSCRIBE_H
#define MQTT_SUBSCRIBE_H

#include <mosquitto.h>

int mqtt_subscribe(struct mosquitto *mosq, int qos);
void mqtt_unsubscribe_callback(struct mosquitto *mosq, void *userdata, int mid);
void mqtt_subscribe_callback(struct mosquitto *mosq, void *userdata, int mid, int qos_count, const int *granted_qos);

#endif
