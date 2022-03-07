#ifndef MQTT_PARSER_H
#define MQTT_PARSER_H

#include "common.h"

#define DEFAULT_KEEPALIVE 60
#define DEFAULT_QOS 0

int mqtt_parse_config_file(char *configFile, mqtt_ctx_t *mqtt_ctx);

#endif
