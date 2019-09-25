#include "LoggerStderr.h"

LoggerStderr::LoggerStderr()
{

}

LoggerStderr::LoggerStderr(const uint32_t level):
    LoggerInterface(level)
{

}

LoggerStderr::~LoggerStderr()
{

}

int LoggerStderr::print_it_output(const std::string &str)
{
    std::cerr << str << std::endl;
    return 0;
}
