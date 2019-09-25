#ifndef LOGGER_STDOUT_H
#define LOGGER_STDOUT_H
#include <iostream>
#include "LoggerInterface.h"

class LoggerStdout : public LoggerInterface
{
public:
    LoggerStdout();
    LoggerStdout(const uint32_t level);
    virtual ~LoggerStdout ();

    virtual int print_it_output(const std::string &str);
private:
};
#endif /* ifndef LOGGER_STDOUT_H */
