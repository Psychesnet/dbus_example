#ifndef DBUSSERVER_H
#define DBUSSERVER_H

#include <pthread.h>
#include "DBusClient.h"

class DBusServer : public DBusClient
{
public:
    explicit DBusServer();
    virtual ~DBusServer();

    int deinit() override;

    virtual int start_listen_thread();
    virtual int stop_listen_thread();

private:
    static void *start_listen_service(void *args);
    int listen_loop();
    void reply_to_method_call(DBusMessage * msg);

protected:
    pthread_t pid;
};

#endif /* DBUSSERVER_H */
