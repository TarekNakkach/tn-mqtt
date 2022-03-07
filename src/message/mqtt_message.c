#include "mqtt_message.h"
#include "common.h"

static int message_get_topic(char *topic)
{
    if (strncmp(topic, TOPICS_LIGHT, strlen(topic)) == 0) {
        return TOPIC_LIGHT;
    } else if (strncmp(topic, TOPICS_MICROWAVE, strlen(topic)) == 0) {
        return TOPIC_MICROWAVE;
    } else {
        return TOPIC_UNKNOWN;
    }
}

static int message_get_state(char *payload)
{
    if (strncmp(payload, "off", strlen(payload)) == 0) {
        return STATE_OFF;
    } else if (strncmp(payload, "on", strlen(payload)) == 0) {
        return STATE_ON;
    } else {
        return STATE_UNKNOWN;
    }
}

void mqtt_message_send_response(struct mosquitto *mosq, const struct mosquitto_message *message)
{
    int topic = message_get_topic(message->topic);
    int state = message_get_state((char*) message->payload);


    switch (topic)
    {
        case TOPIC_LIGHT:
            {
                if (state == STATE_ON) {
                    char *msg = "Pulish Client Light On";
                    mosquitto_publish(mosq, NULL, message->topic, strlen(msg), msg, 0, 0);
                } // @TODO : add off state
                break;
            }
        case TOPIC_MICROWAVE:
            {
                if (state == STATE_ON) {
                    char *msg = "Pulish Client Microwave On";
                    mosquitto_publish(mosq, NULL, message->topic, strlen(msg), msg, 0, 0);
                } // @TODO : add off state
                break;
            }
        default: return;
    }
}

void mqtt_message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
    if (message->payloadlen)
    {
        LOG(LOG_INFO, "######### Recived Message (topic : '%s'  payload : '%s')", message->topic, (char*) message->payload);
        mqtt_message_send_response(mosq, message);
    }
    fflush(stdout);
}
