#include "DBusClient.h"

int main(int argc, char *argv[])
{
    int ret = 0;
    DBusClient client;
    do {
        while (true) {
            sleep(1);
            log_info("%s try to send something", DAEMON_SHEEP);
            if (client.init(DAEMON_SHEEP) != 0) {
                log_error("init dameon client failed");
                break;
            }
            if (client.create_transation(DAEMON_PRAWN, DBUS_TYPE_METHOD) != 0) {
                log_error("create transation to server failed");
                break;
            }
            if (client.append_string_transation("Hello, I'm a sheep.") != 0) {
                log_error("append string failed");
                break;
            }
            if (client.append_integer32_transation(100) != 0) {
                log_error("append int32 failed");
                break;
            }
            if (client.append_integer64_transation(2^64-1) != 0) {
                log_error("append int64 failed");
                break;
            }
            if (client.append_string_transation("Good day, method!!") != 0) {
                log_error("append string failed");
                break;
            }
            if ((ret = client.commit_transation_with_reply()) != 0) {
                log_error("commit transation failed");
                break;
            }
            if (client.create_transation(DAEMON_SHEEP, DBUS_TYPE_SIGNAL) != 0) {
                log_error("create transation to server failed");
                break;
            }
            if (client.append_string_transation("Hello, I'm a sheep.") != 0) {
                log_error("append string failed");
                break;
            }
            if (client.append_integer32_transation(132) != 0) {
                log_error("append int32 failed");
                break;
            }
            if (client.append_integer64_transation(2^64-1) != 0) {
                log_error("append int64 failed");
                break;
            }
            if (client.append_string_transation("Good day, signal!!") != 0) {
                log_error("append string failed");
                break;
            }
            if ((ret = client.commit_transation_without_reply()) != 0) {
                log_error("commit transation failed");
                break;
            }
            client.deinit();
            sleep(3);
            log_info("%s try to send something", DAEMON_CROCODILE);
            if (client.init(DAEMON_CROCODILE) != 0) {
                log_error("init dameon client failed");
                break;
            }
            if (client.create_transation(DAEMON_PRAWN, DBUS_TYPE_METHOD) != 0) {
                log_error("create transation to server failed");
                break;
            }
            if (client.append_string_transation("Hello, I'm a crocodile.") != 0) {
                log_error("append string failed");
                break;
            }
            if (client.append_string_transation("Good day, method!!") != 0) {
                log_error("append string failed");
                break;
            }
            if ((ret = client.commit_transation_with_reply()) != 0) {
                log_error("commit transation failed");
                break;
            }
            if (client.create_transation(DAEMON_CROCODILE, DBUS_TYPE_SIGNAL) != 0) {
                log_error("create transation to server failed");
                break;
            }
            if (client.append_string_transation("Hello, I'm a crocodile.") != 0) {
                log_error("append string failed");
                break;
            }
            if (client.append_string_transation("Good day, signal!!") != 0) {
                log_error("append string failed");
                break;
            }
            if ((ret = client.commit_transation_without_reply()) != 0) {
                log_error("commit transation failed");
                break;
            }
            client.deinit();
        }
    } while (false);
    client.deinit();
    return 0;
}
