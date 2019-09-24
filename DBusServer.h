#ifndef DBUSSERVER_H
#define DBUSSERVER_H

#include <pthread.h>
#include "DBusClient.h"

class DBusServer : public DBusClient
{
public:
    explicit DBusServer();
    virtual ~DBusServer();

    virtual int deinit();

    virtual int start_listen_thread();
    virtual int stop_listen_thread();

    // parent will handle common part
    // child should overwrite it and call parent's function
    // when child finish checking
    virtual uint32_t handle_string(const char * param);
    virtual uint32_t handle_uint32(const uint32_t param);
    virtual uint32_t handle_uint64(const uint64_t param);

private:
    static void *start_listen_service(void *args);
    int listen_loop();
    void parse_and_reply(DBusMessage * msg);
    void parse_and_noreply(DBusMessage * msg);

protected:
    pthread_t pid;
};

#endif /* DBUSSERVER_H */
