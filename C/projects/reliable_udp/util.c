#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#define DEBUG 1

void print_debug(const char *format, ...) {
    if (!DEBUG) {
        return;
    }
    va_list args;
    va_start(args, format);
    // Get current time
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    long milliseconds = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
    char time_str[32];
    snprintf(time_str, sizeof(time_str), "%04ld", milliseconds % 10000);
    printf("[%s] ", time_str);
    vprintf(format, args);
    va_end(args);
}

void setnonblocking(int fd) {
    int val;
    if ((val = fcntl(fd, F_GETFL), 0) < 0) {
        perror("fcntl");
    }

    val |= O_NONBLOCK;

    if (fcntl(fd, F_SETFL, val) < 0) {
        perror("fcntl");
    }
}

void setblocking(int fd) {
    int val;
    if ((val = fcntl(fd, F_GETFL, 0)) < 0) {
        perror("fcntl");
        return;
    }

    val &= ~O_NONBLOCK;  // Clear the O_NONBLOCK flag

    if (fcntl(fd, F_SETFL, val) < 0) {
        perror("fcntl");
    }
}
