#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

void daemonize(void) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        /* Parent exits, child continues */
        exit(EXIT_SUCCESS);
    }

    /* Become session leader to detach from terminal */
    if (setsid() < 0) {
        perror("setsid failed");
        exit(EXIT_FAILURE);
    }

    /* Clear file creation mask */
    umask(0);

    /* Change working directory to root */
    if (chdir("/") < 0) {
        perror("chdir failed");
        exit(EXIT_FAILURE);
    }

    /* Close inherited file descriptors */
    long max_fd = sysconf(_SC_OPEN_MAX);
    if (max_fd == -1) {
        max_fd = 1024;
    }
    for (long fd = 0; fd < max_fd; fd++) {
        close(fd);
    }

    /* Redirect stdin, stdout, stderr to /dev/null */
    open("/dev/null", O_RDONLY);
    open("/dev/null", O_RDWR);
    open("/dev/null", O_RDWR);
}

int main(void) {
    FILE *logfp;

    daemonize();

    /* Open log file after daemonization */
    logfp = fopen("/tmp/mydaemon.log", "a+");
    if (!logfp) {
        exit(EXIT_FAILURE);
    }

    /* Periodically write to log */
    while (1) {
        time_t now = time(NULL);
        struct tm *tm_info = localtime(&now);
        char time_str[64];
        strftime(time_str, sizeof(time_str), "%a %b %d %T %Y", tm_info);
        fprintf(logfp, "Mydaemon alive at %s\n", time_str);
        fflush(logfp);
        sleep(5);
    }

    /* Clean up */
    fclose(logfp);
    return EXIT_SUCCESS;
}

