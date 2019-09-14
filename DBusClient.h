#ifndef DBUSCLIENT_H
#define DBUSCLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dbus/dbus.h>
#include <unistd.h>
#include <algorithm>
#include <string>

#include "DaemonService.h"

class DBusClient
{
public:
    explicit DBusClient();
    virtual ~DBusClient();

    virtual int init(const char *service_name);
    virtual int deinit();

    int register_service();
    int send_string(
        const char* host_name,
        const char *key,
        const char *value);

protected:
    bool exit_flag;
    bool already_init;
    DBusError err;
    DBusConnection *conn;
    std::string bus_name;
    std::string object_name;
    std::string interface_name;
};

#endif /* DBUSCLIENT_H */
