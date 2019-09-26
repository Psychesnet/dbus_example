#ifndef BACKTRACE_H
#define BACKTRACE_H
#include <signal.h>
#include <execinfo.h>
#include <errno.h>
#include <cxxabi.h>
#include "Logger.h"

class Backtrace
{
    public:
        Backtrace();
        virtual ~Backtrace();

    private:
        void register_backtrace();
        static void handle_backtrace(int signum
#ifdef Darwin
                ,
                struct __siginfo* act,
                void *oact
#endif
                );

    private:
};
#endif /* ifndef BACKTRACE_H */
