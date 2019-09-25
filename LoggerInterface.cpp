#include "LoggerInterface.h"

LoggerInterface::LoggerInterface()
{
    default_log_level = LOGGER_LEVEL_INFO;
}

LoggerInterface::LoggerInterface(const uint32_t level)
{
    default_log_level = level;
}

LoggerInterface::~LoggerInterface()
{

}

int LoggerInterface::change_level(const uint32_t level)
{
    if (level < LOGGER_LEVEL_NUMS) {
        default_log_level = level;
        return 0;
    }
    return -1;
}

int LoggerInterface::debug(const char *output)
{
    if (default_log_level <= LOGGER_LEVEL_DEBUG) {
        return prepare_timestamp(LOGGER_COLOR_GREEN, output);
    }
    return 0;
}

int LoggerInterface::info(const char *output)
{
    if (default_log_level <= LOGGER_LEVEL_INFO) {
        return prepare_timestamp(LOGGER_COLOR_DEFAULT, output);
    }
    return 0;
}

int LoggerInterface::warning(const char *output)
{
    if (default_log_level <= LOGGER_LEVEL_WARNING) {
        return prepare_timestamp(LOGGER_COLOR_YELLOW, output);
    }
    return 0;
}

int LoggerInterface::error(const char *output)
{
    if (default_log_level <= LOGGER_LEVEL_ERROR) {
        return prepare_timestamp(LOGGER_COLOR_RED, output);
    }
    return 0;
}

int LoggerInterface::critical(const char *output)
{
    if (default_log_level <= LOGGER_LEVEL_CRITICAL) {
        return prepare_timestamp(LOGGER_COLOR_BLUE, output);
    }
    return 0;
}

int LoggerInterface::prepare_timestamp(const char *color, const char *str)
{
    struct timeval tmnow;
    struct tm tstruct;
    char buf[80] = {0};
    gettimeofday(&tmnow, NULL);
    tstruct = *localtime(&tmnow.tv_sec);
    snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d.%06d",
            tstruct.tm_year+1970,
            tstruct.tm_mon,
            tstruct.tm_mday,
            tstruct.tm_hour,
            tstruct.tm_min,
            tstruct.tm_sec,
            tmnow.tv_usec);

    return print_it_output(
            std::string(color)+
            std::string("[")+
            std::string(buf)+
            std::string("]")+
            std::string(str)+
            std::string(LOGGER_COLOR_RESET));
}

