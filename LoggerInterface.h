#ifndef LOGGER_INTERFACE_H
#define LOGGER_INTERFACE_H
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <sys/time.h>
#include <string>
#include <chrono>

#define LOGGER_LEVEL_DEBUG    0
#define LOGGER_LEVEL_INFO     1
#define LOGGER_LEVEL_WARNING  2
#define LOGGER_LEVEL_ERROR    3
#define LOGGER_LEVEL_CRITICAL 4
#define LOGGER_LEVEL_NUMS     5

#define LOGGER_COLOR_GREEN    "\033[0;32m"
#define LOGGER_COLOR_DEFAULT  "\033[0;39m"
#define LOGGER_COLOR_YELLOW   "\033[0;33m"
#define LOGGER_COLOR_RED      "\033[0;31m"
#define LOGGER_COLOR_BLUE     "\033[0;34m"
#define LOGGER_COLOR_RESET    "\033[0m"

#define LOGGER_TYPE_STDOUT    0
#define LOGGER_TYPE_STDERR    1
#define LOGGER_TYPE_SYSLOG    2

class LoggerInterface
{
    public:
        LoggerInterface();
        LoggerInterface(const uint32_t level);
        virtual ~LoggerInterface();

        int change_level(const uint32_t level);
        int debug(const char *output);
        int info(const char *output);
        int warning(const char *output);
        int error(const char *output);
        int critical(const char *output);

        virtual int prepare_timestamp(const char *color, const char *str);
        virtual int print_it_output(const std::string &str) = 0;

    private:
        uint32_t default_log_level;
};
#endif /* ifndef LOGGER_INTERFACE_H */
