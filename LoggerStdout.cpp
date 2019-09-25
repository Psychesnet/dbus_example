#include "LoggerStdout.h"

LoggerStdout::LoggerStdout()
{

}

LoggerStdout::LoggerStdout(const uint32_t level):
    LoggerInterface(level)
{

}

LoggerStdout::~LoggerStdout()
{

}

int LoggerStdout::print_it_output(const std::string &str)
{
    std::cout << "\033[0;31m" << str << std::endl;
    return 0;
}
