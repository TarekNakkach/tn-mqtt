#ifndef COMMMON_H
#define COMMMON_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <syslog.h>

#define LOG(prio, fmt, args...) syslog(prio,fmt " (%s@%s:%d) ", ##args, __FILE__, __FUNCTION__, __LINE__)

typedef struct
{
    char *address;
    int port;
    int keepalive;
    int qos;
    bool clean_session;
} mqtt_ctx_t;

mqtt_ctx_t mqtt_ctx;

#endif
