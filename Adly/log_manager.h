#ifndef LOG_MANAGER_H
#define LOG_MANAGER_H

#include <Windows.h>
#include <stdio.h>
#include <stdarg.h>

typedef enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_CRITICAL
} log_level_t;

typedef struct {
    void* prvt;

    // Log methods for different levels
    void (*log_debug)(const char* format, ...);
    void (*log_info)(const char* format, ...);
    void (*log_warn)(const char* format, ...);
    void (*log_error)(const char* format, ...);
    void (*log_critical)(const char* format, ...);
} log_manager_t;

// Function to access the singleton instance
log_manager_t* log_manager_instance(void);

// Cleanup function
void log_manager_cleanup(void);

// Macro definitions for single-line log method calls
#define LOG_DEBUG(fmt, ...)      log_manager_instance()->log_debug(fmt, __VA_ARGS__)
#define LOG_INFO(fmt, ...)       log_manager_instance()->log_info(fmt, __VA_ARGS__)
#define LOG_WARN(fmt, ...)       log_manager_instance()->log_warn(fmt, __VA_ARGS__)
#define LOG_ERROR(fmt, ...)      log_manager_instance()->log_error(fmt, __VA_ARGS__)
#define LOG_CRITICAL(fmt, ...)   log_manager_instance()->log_critical(fmt, __VA_ARGS__)

#endif // LOG_MANAGER_H
