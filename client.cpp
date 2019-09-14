#include "DBusClient.h"

int main(int argc, char *argv[])
{
    DBusClient client;
    do {
        while (true) {
            sleep(1);
            if (client.init(DAEMON_SHEEP) != 0) {
                fprintf(stderr, "init dameon client failed\n");
                break;
            }
            if (client.send_string("server", HANDLE_METHOD, "Hello, I'm a sheep.") != 0) {
                fprintf(stderr, "send string to server failed\n");
                break;
            }
            client.deinit();
            sleep(3);
            if (client.init(DAEMON_CROCODILE) != 0) {
                fprintf(stderr, "init dameon client failed\n");
                break;
            }
            if (client.send_string("server", HANDLE_METHOD, "Hello, I'm a crocodile.") != 0) {
                fprintf(stderr, "send string to server failed\n");
                break;
            }
            client.deinit();
        }
    } while (false);
    client.deinit();
    return 0;
}
