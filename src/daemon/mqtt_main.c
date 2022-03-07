#include <mosquitto.h>

#include "common.h"
#include "mqtt_subscribe.h"
#include "mqtt_daemonize.h"
#include "mqtt_message.h"
#include "mqtt_parser.h"

static void mqtt_connect_callback(struct mosquitto *mosq, void *userdata, int result)
{
    if (!result) {
        mqtt_subscribe(mosq, mqtt_ctx.qos);
    } else {
        LOG(LOG_ERR, "######### Connect failed");
    }
}

static void mqtt_log_callback(struct mosquitto *mosq, void *userdata, int level, const char *str)
{
    LOG(LOG_INFO, "######### Log : %s", str);
}

static int mqtt_usage(char *exec)
{
    printf("Usage: %s [ARGS]\n", exec);
    printf("Args:\n");
    printf("\t-h Print this help\n");
    printf("\t-d Run as daemon\n");
    printf("\t-c Json config file path : i.e config/config.json\n");
    printf("\t-l Syslog debug level : 0 to disable logs, 7 to enable all logs\n");
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        return mqtt_usage(argv[0]);
    }

    int args;
    char *configFile = NULL;
    int debugLevel = LOG_ERR;
    bool daemonize = false;

    while ((args = getopt(argc, argv, "hdc:l:")) != -1) {
        switch (args)
        {
            case 'd':
                daemonize = true;
                break;
            case 'c':
                configFile = optarg;
                break;
            case 'l':
                debugLevel = atoi(optarg);
                break;
            case 'h':
            default:
                return mqtt_usage(argv[0]);
        }
    }

    openlog(argv[0], LOG_CONS|LOG_PID, LOG_DAEMON);
    setlogmask(LOG_UPTO(debugLevel));

    if (mqtt_parse_config_file(configFile, &mqtt_ctx) != EXIT_SUCCESS) {
        goto out;
    }

    if (daemonize) {
        mqtt_daemonize();
    }
    
    mosquitto_lib_init();

    struct mosquitto *mosq = NULL;
    mosq = mosquitto_new(argv[0], mqtt_ctx.clean_session, NULL);
    if (!mosq) {
        LOG(LOG_ERR, "######### Failed to create mosquitto instance");
        goto out;
    }

    mosquitto_log_callback_set(mosq, mqtt_log_callback);
    mosquitto_connect_callback_set(mosq, mqtt_connect_callback);
    mosquitto_message_callback_set(mosq, mqtt_message_callback);
    mosquitto_subscribe_callback_set(mosq, mqtt_subscribe_callback);
    mosquitto_unsubscribe_callback_set(mosq, mqtt_unsubscribe_callback);

    LOG(LOG_NOTICE, "######### Start connection to '%s' on port '%d'", mqtt_ctx.address, mqtt_ctx.port);

    if (mosquitto_connect(mosq, mqtt_ctx.address, mqtt_ctx.port, mqtt_ctx.keepalive)) {
        LOG(LOG_ERR, "######### %s", strerror(errno));
        goto out;
    } else {
        free(mqtt_ctx.address);
    }

    mosquitto_loop_forever(mosq, -1, 1);

    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
out:
    if (mqtt_ctx.address) free(mqtt_ctx.address);
    closelog();

    return EXIT_SUCCESS;
}
