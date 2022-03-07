#ifndef MQTT_MESSAGE_H
#define MQTT_MESSAGE_H

#include <mosquitto.h>

#define TOPICS_LIGHT "HomeIoT/light"
#define TOPICS_MICROWAVE "HomeIoT/microwave"

enum topic_t {
    TOPIC_UNKNOWN,
    TOPIC_LIGHT,
    TOPIC_MICROWAVE
};

enum state_t {
    STATE_OFF,
    STATE_ON,
    STATE_UNKNOWN
}; 

void mqtt_message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message);

#endif

