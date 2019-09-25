#include "Logger.h"

static LoggerInterface *log = NULL;
int logger_init(const int type)
{
    int ret = -1;
    switch (type) {
        case LOGGER_TYPE_STDOUT:
            log = new LoggerStdout();
            break;
        case LOGGER_TYPE_STDERR:
            log = new LoggerStderr();
            break;
        case LOGGER_TYPE_SYSLOG:
            log = new LoggerSyslog();
            break;
        default:
           break;
    }
    return ret;
}

int logger_change_log_type(const int type)
{
    logger_deinit();
    return logger_init(type);
}

int logger_change_log_level(const int level)
{
    if (log) {
        return log->change_level(level);
    }
    return -1;
}

int logger_print_it(const uint32_t level, const char *format, ...)
{
    int ret = -1;
    if (!log || (level >= LOGGER_LEVEL_NUMS)) {
        return ret;
    }
    char output[1024] = {0};
    va_list arg;
    va_start(arg, format);
    vsprintf(output, format, arg);
    switch (level) {
        case LOGGER_LEVEL_DEBUG:
            ret = log->debug(output);
            break;
        case LOGGER_LEVEL_INFO:
            ret = log->info(output);
            break;
        case LOGGER_LEVEL_WARNING:
            ret = log->warning(output);
            break;
        case LOGGER_LEVEL_ERROR:
            ret = log->error(output);
            break;
        case LOGGER_LEVEL_CRITICAL:
            ret = log->critical(output);
            break;
        default:
            break;
    }
    va_end(arg);
    return ret;
}

int logger_deinit()
{
    if (log) {
        delete log;
        log = NULL;
    }
    return 0;
}
