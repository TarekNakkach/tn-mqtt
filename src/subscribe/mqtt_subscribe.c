#include "mqtt_subscribe.h"
#include "common.h"

int mqtt_subscribe(struct mosquitto *mosq, int qos)
{
    return mosquitto_subscribe(mosq, NULL, "HomeIoT/#", qos);
}

void mqtt_subscribe_callback(struct mosquitto *mosq, void *userdata, int mid, int qos_count, const int *granted_qos)
{
    LOG(LOG_INFO, "######### Subscribed (mid: %d): %d", mid, granted_qos[0]);
    for (int i = 1; i < qos_count; i++) {
        LOG(LOG_INFO, "%d", granted_qos[i]);
    }
}

void mqtt_unsubscribe_callback(struct mosquitto *mosq, void *userdata, int mid)
{
    LOG(LOG_INFO, "######### Unsubscribed (mid: %d)", mid);
}
