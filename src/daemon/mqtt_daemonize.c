#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "mqtt_daemonize.h"
#include "common.h"

void mqtt_daemonize()
{
    pid_t pid;
    pid = fork();
    if (pid < 0)
        exit(EXIT_FAILURE);

    if (pid > 0)
        exit(EXIT_SUCCESS);

    if (setsid() < 0)
        exit(EXIT_FAILURE);

    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    pid = fork();
    if (pid < 0)
        exit(EXIT_FAILURE);

    if (pid > 0)
        exit(EXIT_SUCCESS);

    umask(0);
    chdir("/");

    for (int fd = sysconf(_SC_OPEN_MAX); fd>=0; fd--) {
        close(fd);
    }
}
