#ifndef LOGGER_SYSLOG_H
#define LOGGER_SYSLOG_H
#include <syslog.h>
#include "LoggerInterface.h"

class LoggerSyslog : public LoggerInterface
{
public:
    LoggerSyslog();
    LoggerSyslog(const uint32_t level);
    virtual ~LoggerSyslog ();

    virtual int print_it_output(const std::string &str);
private:
};
#endif /* ifndef LOGGER_SYSLOG_H */
