#include "DBusServer.h"

int main(int argc, char *argv[])
{
    DBusServer server;
    do {
        if (server.init(DAEMON_PRAWN) != 0) {
            log_error("init server failed\n");
            break;
        }
        if (server.start_listen_thread() != 0) {
            log_error("start thread failed\n");
            break;
        }
        log_info("start listen service: %s", DAEMON_PRAWN);
        while (true) {
            sleep(1);
        }
    } while (false);
    server.deinit();
    return 0;
}
