#include "log_manager.h"
#include <stdlib.h>
#include <time.h>

#define LM_PRVT ((log_manager_prvt_t*)lm_instance->prvt)

typedef struct {
    FILE* file;
} log_manager_prvt_t;

// Static variable for the singleton instance
static log_manager_t* lm_instance = NULL;

static void log_to_console(const char* level, const char* format, va_list args)
{
    printf("[%s] ", level);
    vprintf(format, args);
    printf("\n");
}

static void log_to_file(const char* level, const char* format, va_list args)
{
    if (LM_PRVT->file) {
        time_t t = time(NULL);
        struct tm tm_info;
        char time_buffer[26];

        localtime_s(&tm_info, &t);
        strftime(time_buffer, 26, "%Y-%m-%d %H:%M:%S", &tm_info);

        fprintf(LM_PRVT->file, "[%s] [%s] ", time_buffer, level);
        vfprintf(LM_PRVT->file, format, args);
        fprintf(LM_PRVT->file, "\n");
        fflush(LM_PRVT->file);
    }
}

// Updated DEFINE_LOG_FUNCTION to accept log level as a parameter
#define DEFINE_LOG_FUNCTION(function_name, level_str, level_enum)        \
    static void log_##function_name##_impl(const char* format, ...) {    \
        va_list args;                                                    \
        va_start(args, format);                                          \
        log_to_console(level_str, format, args);                         \
        if (level_enum == LOG_ERROR || level_enum == LOG_CRITICAL) {     \
            log_to_file(level_str, format, args);                        \
        }                                                                \
        va_end(args);                                                    \
    }

// Define log functions with specific levels and corresponding text labels
DEFINE_LOG_FUNCTION(debug, "DEBUG", LOG_DEBUG)
DEFINE_LOG_FUNCTION(info, "INFO", LOG_INFO)
DEFINE_LOG_FUNCTION(warn, "WARN", LOG_WARN)
DEFINE_LOG_FUNCTION(error, "ERROR", LOG_ERROR)
DEFINE_LOG_FUNCTION(critical, "CRITICAL", LOG_CRITICAL)

log_manager_t* log_manager_instance(void)
{
    if (lm_instance == NULL) {

        lm_instance = (log_manager_t*)malloc(sizeof(log_manager_t));
        if (lm_instance == NULL) {
            exit(1);
        }

        lm_instance->prvt = malloc(sizeof(log_manager_prvt_t));
        if (lm_instance->prvt == NULL) {
            free(lm_instance);
            exit(1);
        }

        // Initialize file for critical logs
        errno_t err = fopen_s(&LM_PRVT->file, "critical_logs.txt", "a");
        if (err != 0) {
            perror("Failed to open critical log file");
        }

        // Set log methods
        lm_instance->log_debug    = log_debug_impl;
        lm_instance->log_info     = log_info_impl;
        lm_instance->log_warn     = log_warn_impl;
        lm_instance->log_error    = log_error_impl;
        lm_instance->log_critical = log_critical_impl;

        // Allocate console
        if (AllocConsole()) {
            FILE* consoleStream;
            freopen_s(&consoleStream, "CONOUT$", "w", stdout);
            freopen_s(&consoleStream, "CONOUT$", "w", stderr);
            // Set wide-character title for the console window
            SetConsoleTitleW(L"Debug Console - Wide Character Title");
        }
    }

    return lm_instance;
}

// Cleanup function for log manager
void log_manager_cleanup(void)
{
    if (LM_PRVT->file) {
        fclose(LM_PRVT->file);
    }
    FreeConsole();
    free(LM_PRVT);
    free(lm_instance);
    lm_instance = NULL;
}