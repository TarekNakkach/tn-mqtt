#include <json-c/json.h>
#include "mqtt_parser.h"

int mqtt_parse_config_file(char *configFile, mqtt_ctx_t *mqtt_ctx)
{
    FILE * fp;
    char buffer[1024];
    struct json_object * parsed_json = NULL;
    struct json_object *json_address, *json_port, *json_keepalive, *json_qos, *json_clean_session;

    fp = fopen(configFile, "r");
    if (fp == NULL) {
        LOG(LOG_ERR, "######### Failed to open config file '%s' : %s", configFile, strerror(errno));
        return EXIT_FAILURE;
    }

    size_t read_cnt = fread(buffer, 1024, 1, fp);
    fclose(fp);

    if (read_cnt < 0) {
        LOG(LOG_ERR, "######### Failed to read config file '%s' : %s", configFile, strerror(errno));
        return EXIT_FAILURE;
    }

    parsed_json = json_tokener_parse(buffer);
    if (parsed_json == NULL) {
        LOG(LOG_ERR, "######### Failed to create parser for config file %s", configFile);
        return EXIT_FAILURE;
    }

    if (json_object_object_get_ex(parsed_json, "address", &json_address)) {
        mqtt_ctx->address = strdup(json_object_get_string(json_address));
    } else {
        LOG(LOG_ERR, "######### Failed to parse 'address'");
        return EXIT_FAILURE;
    }

    if (json_object_object_get_ex(parsed_json, "port", &json_port)) {
        mqtt_ctx->port = json_object_get_int(json_port);
    } else {
        LOG(LOG_ERR, "######### Failed to parse 'port'");
        return EXIT_FAILURE;
    }

    if (json_object_object_get_ex(parsed_json, "keepalive", &json_keepalive)) {
        mqtt_ctx->keepalive = json_object_get_int(json_keepalive);
    } else {
        LOG(LOG_CRIT, "######### Failed to parse 'keepalive', set to %d", DEFAULT_KEEPALIVE);
        mqtt_ctx->keepalive = DEFAULT_KEEPALIVE;
    }

    if (json_object_object_get_ex(parsed_json, "qos", &json_qos)) {
        mqtt_ctx->qos = json_object_get_int(json_qos);
    } else {
        LOG(LOG_CRIT, "######### Failed to parse 'qos', set to %d", DEFAULT_QOS);
        mqtt_ctx->qos = DEFAULT_QOS;
    }

    if (json_object_object_get_ex(parsed_json, "clean_session", &json_clean_session)) {
        mqtt_ctx->clean_session = json_object_get_boolean(json_clean_session);
    } else {
        LOG(LOG_CRIT, "######### Failed to parse 'clean_session', set to true");
        mqtt_ctx->clean_session = true;
    }

    return EXIT_SUCCESS;
}
