#include "DBusClient.h"

int main(int argc, char *argv[])
{
    int ret = 0;
    DBusClient client;
    do {
        while (true) {
            sleep(1);
            if (client.init(DAEMON_SHEEP) != 0) {
                fprintf(stderr, "init dameon client failed\n");
                break;
            }
            if (client.create_transation(DAEMON_PRAWN, DBUS_TYPE_METHOD) != 0) {
                fprintf(stderr, "create transation to server failed\n");
                break;
            }
            if (client.append_string_transation("Hello, I'm a sheep.") != 0) {
                fprintf(stderr, "append string failed\n");
                break;
            }
            if (client.append_integer32_transation(100) != 0) {
                fprintf(stderr, "append int32 failed\n");
                break;
            }
            if (client.append_integer64_transation(2^64-1) != 0) {
                fprintf(stderr, "append int64 failed\n");
                break;
            }
            if (client.append_string_transation("Good day, method!!") != 0) {
                fprintf(stderr, "append string failed\n");
                break;
            }
            if ((ret = client.commit_transation_with_reply()) != 0) {
                fprintf(stderr, "commit transation failed\n");
                break;
            }
            if (client.create_transation(DAEMON_SHEEP, DBUS_TYPE_SIGNAL) != 0) {
                fprintf(stderr, "create transation to server failed\n");
                break;
            }
            if (client.append_string_transation("Hello, I'm a sheep.") != 0) {
                fprintf(stderr, "append string failed\n");
                break;
            }
            if (client.append_integer32_transation(132) != 0) {
                fprintf(stderr, "append int32 failed\n");
                break;
            }
            if (client.append_integer64_transation(2^64-1) != 0) {
                fprintf(stderr, "append int64 failed\n");
                break;
            }
            if (client.append_string_transation("Good day, signal!!") != 0) {
                fprintf(stderr, "append string failed\n");
                break;
            }
            if ((ret = client.commit_transation_without_reply()) != 0) {
                fprintf(stderr, "commit transation failed\n");
                break;
            }
            client.deinit();
            fprintf(stderr, "return %d\n", ret);
            sleep(3);
            if (client.init(DAEMON_CROCODILE) != 0) {
                fprintf(stderr, "init dameon client failed\n");
                break;
            }
            if (client.create_transation(DAEMON_PRAWN, DBUS_TYPE_METHOD) != 0) {
                fprintf(stderr, "create transation to server failed\n");
                break;
            }
            if (client.append_string_transation("Hello, I'm a crocodile.") != 0) {
                fprintf(stderr, "append string failed\n");
                break;
            }
            if (client.append_string_transation("Good day, method!!") != 0) {
                fprintf(stderr, "append string failed\n");
                break;
            }
            if ((ret = client.commit_transation_with_reply()) != 0) {
                fprintf(stderr, "commit transation failed\n");
                break;
            }
            if (client.create_transation(DAEMON_CROCODILE, DBUS_TYPE_SIGNAL) != 0) {
                fprintf(stderr, "create transation to server failed\n");
                break;
            }
            if (client.append_string_transation("Hello, I'm a crocodile.") != 0) {
                fprintf(stderr, "append string failed\n");
                break;
            }
            if (client.append_string_transation("Good day, signal!!") != 0) {
                fprintf(stderr, "append string failed\n");
                break;
            }
            if ((ret = client.commit_transation_without_reply()) != 0) {
                fprintf(stderr, "commit transation failed\n");
                break;
            }
            client.deinit();
            fprintf(stderr, "return %d\n", ret);
        }
    } while (false);
    client.deinit();
    return 0;
}
