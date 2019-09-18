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

private:
    static void *start_listen_service(void *args);
    int listen_loop();
    void parse_and_reply(DBusMessage * msg);

protected:
    pthread_t pid;
};

#endif /* DBUSSERVER_H */
