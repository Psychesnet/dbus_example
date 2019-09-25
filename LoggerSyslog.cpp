#include "LoggerSyslog.h"

LoggerSyslog::LoggerSyslog()
{
    openlog(getprogname(), LOG_CONS | LOG_PID, 0);
}

LoggerSyslog::LoggerSyslog(const uint32_t level):
    LoggerInterface(level)
{
    LoggerSyslog();
}

LoggerSyslog::~LoggerSyslog()
{

    closelog();
}

int LoggerSyslog::print_it_output(const std::string &str)
{
    syslog(LOG_SYSLOG, "%s", str.c_str());
    return 0;
}
