#ifndef __DEBUG_H
#define __DEBUG_H

#include <stdio.h>
#include <time.h>
#include <string.h>

#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_WARN  1
#define LOG_LEVEL_INFO  2
#define LOG_LEVEL_DEBUG 3

extern int current_log_level;

#define LOG(level, fmt, ...) \
    do { \
        if (level <= current_log_level) { \
            time_t now = time(NULL); \
            char *time_str = ctime(&now); \
            if (time_str) { \
                time_str[strlen(time_str)-1] = '\0'; \
                fprintf(stderr, "[%s] [%s] " fmt "\n", \
                        time_str, \
                        level == LOG_LEVEL_ERROR ? "ERROR" : \
                        level == LOG_LEVEL_WARN ? "WARN" : \
                        level == LOG_LEVEL_INFO ? "INFO" : "DEBUG", \
                        ##__VA_ARGS__); \
            } \
        } \
    } while(0)

#define GPIO_DEBUG(fmt, ...) LOG(LOG_LEVEL_DEBUG, "[GPIO] " fmt, ##__VA_ARGS__)
#define GPIO_INFO(fmt, ...) LOG(LOG_LEVEL_INFO, "[GPIO] " fmt, ##__VA_ARGS__)
#define GPIO_WARN(fmt, ...) LOG(LOG_LEVEL_WARN, "[GPIO] " fmt, ##__VA_ARGS__)
#define GPIO_ERROR(fmt, ...) LOG(LOG_LEVEL_ERROR, "[GPIO] " fmt, ##__VA_ARGS__)

#endif /* __DEBUG_H */ 