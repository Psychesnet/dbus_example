#ifndef LOGGER_H
#define LOGGER_H
#include "LoggerInterface.h"
#include "LoggerStdout.h"
#include "LoggerStderr.h"
#include "LoggerSyslog.h"

int logger_init(const int type);
int logger_change_log_type(const int type);
int logger_change_log_level(const int level);
int logger_print_it(const uint32_t level, const char *format, ...);
int logger_deinit();

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define log_debug(format, ...) \
    logger_print_it(LOGGER_LEVEL_DEBUG, "[   DEBUG][%s:%s@%04d]" format, __FILE__, __func__, __LINE__, ##__VA_ARGS__);
#define log_info(format, ...) \
    logger_print_it(LOGGER_LEVEL_INFO, "[    INFO][%s:%s@%04d]" format, __FILE__, __func__, __LINE__, ##__VA_ARGS__);
#define log_warning(format, ...) \
    logger_print_it(LOGGER_LEVEL_WARNING, "[ WARNING][%s:%s@%04d]" format, __FILE__, __func__, __LINE__, ##__VA_ARGS__);
#define log_error(format, ...) \
    logger_print_it(LOGGER_LEVEL_ERROR, "[   ERROR][%s:%s@%04d]" format, __FILE__, __func__, __LINE__, ##__VA_ARGS__);
#define log_critical(format, ...) \
    logger_print_it(LOGGER_LEVEL_CRITICAL, "[CRITICAL][%s:%s@%04d]" format, __FILE__, __func__, __LINE__, ##__VA_ARGS__);
#endif /* ifndef LOGGER_H */
