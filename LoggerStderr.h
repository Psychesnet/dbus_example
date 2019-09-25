#ifndef LOGGER_STDERR_H
#define LOGGER_STDERR_H
#include <iostream>
#include "LoggerInterface.h"

class LoggerStderr : public LoggerInterface
{
public:
    LoggerStderr();
    LoggerStderr(const uint32_t level);
    virtual ~LoggerStderr ();

    virtual int print_it_output(const std::string &str);
private:
};
#endif /* ifndef LOGGER_STDERR_H */
