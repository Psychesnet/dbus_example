#include "DBusServer.h"

int main(int argc, char *argv[])
{
    DBusServer server;
    do {
        if (server.init(DAEMON_PRAWN) != 0) {
            fprintf(stderr, "init server failed\n");
            break;
        }
        if (server.start_listen_thread() != 0) {
            fprintf(stderr, "start thread failed\n");
            break;
        }
        while (true) {
            sleep(1);
        }
    } while (false);
    server.deinit();
    return 0;
}
