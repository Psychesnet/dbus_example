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

    const char *get_support_method(const DBUS_METHOD_INDEX index);

    int register_service();
    // client can create multiple msg into one transation
    int create_transation(const char* host_name, const DBUS_METHOD_INDEX index);
    int append_string_transation(const char *value);
    int append_integer32_transation(const int32_t value);
    int append_integer64_transation(const int64_t value);
    int append_float_transation(const float value);
    int commit_transation();
    int abort_transation();

protected:
    bool exit_flag;
    bool already_init;
    DBusError err;
    DBusConnection *conn;
    std::string bus_name;
    std::string object_name;
    std::string interface_name;
    DBusMessage *dbus_msg;
	DBusMessageIter send_items;
};

#endif /* DBUSCLIENT_H */
