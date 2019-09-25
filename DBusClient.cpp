#include "DBusClient.h"

DBusClient::DBusClient()
{
    already_init = false;
    conn = NULL;
    dbus_msg = NULL;
    logger_init(LOGGER_TYPE_STDOUT);
}

DBusClient::~DBusClient()
{
    logger_deinit();
}

int DBusClient::init(const char *service)
{
    int ret = -1;
    do {
        if (already_init) {
            log_error("alreay init service");
            break;
        }
        dbus_error_init(&err);
        // set up variables
        service_name = std::string(service);
        bus_name = std::string(BASIC_BUS_NAME) + std::string(service);
        // register it
        register_service();
        already_init = true;
        exit_flag = false;
        log_info("Init all resources");
        ret = 0;
    } while (false);
    return ret;
}

int DBusClient::deinit()
{
    if (already_init) {
        dbus_error_free(&err);
        already_init = false;
    }
    log_info("Release all resources");
    exit_flag = true;
    return 0;
}

int DBusClient::register_service()
{
    int ret = -1;
    do {
        // connect to dbus
        conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
        if (dbus_error_is_set(&err)) {
            log_error("Get session failed, %s", err.message);
            break;
        }
        if (!conn) {
            log_error("Connection is empty");
            break;
        }
        // set up unique bus name
        ret = dbus_bus_request_name(conn, bus_name.c_str(), DBUS_NAME_FLAG_REPLACE_EXISTING, &err);
        if (dbus_error_is_set(&err)) {
            log_error("Request bus name failed, %s", err.message);
            break;
        }
        if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
            log_error("Not bus name owner");
            break;
        }
        log_info("Finish request dbus session");
        ret = 0;
    } while (false);
    return ret;
}

int DBusClient::create_transation(const char *host_name, const DBUS_TYPE_INDEX index)
{
    int ret = -1;
    do {
        std::string target_bus_name;
        switch (index) {
            case DBUS_TYPE_METHOD:
                log_debug("create transation with method");
                target_bus_name = std::string(BASIC_BUS_NAME) + std::string(host_name);
                object_name = "/" + target_bus_name + "/object";
                std::replace(object_name.begin(), object_name.end(), '.', '/');
                interface_name = target_bus_name + ".interface";
                dbus_msg = dbus_message_new_method_call(target_bus_name.c_str(), object_name.c_str(),
                        interface_name.c_str(), get_support_type(index));
                break;
            case DBUS_TYPE_SIGNAL:
                log_debug("create transation with signal");
                object_name = "/" + std::string(BASIC_SIGNAL_NAME) + "/object";
                std::replace(object_name.begin(), object_name.end(), '.', '/');
                interface_name = std::string(BASIC_SIGNAL_NAME) + ".interface";
                dbus_msg = dbus_message_new_signal(object_name.c_str(),
                        interface_name.c_str(), get_support_type(index));
                break;
            default:
                break;
        }
        if (!dbus_msg) {
            log_error("Create message failed, %s", err.message);
            break;
        }
        log_debug("type: %s", get_support_type(index));
        log_debug("object: %s", object_name.c_str());
        log_debug("interface: %s", interface_name.c_str());
        // init container and link to dbus_msg
        dbus_message_iter_init_append(dbus_msg, &send_items);
        ret = 0;
    } while (false);
    return ret;
}

int DBusClient::append_string_transation(const char *value)
{
    int ret = -1;
    do {
        if (!dbus_msg) {
            log_error("need to create transation first");
            break;
        }
        // put string into container
        if(!dbus_message_iter_append_basic(&send_items, DBUS_TYPE_STRING, &value)) {
            log_error("Append string failed");
            break;
        }
        log_debug("append string %s", value);
        ret = 0;
    } while (false);
    return ret;
}

int DBusClient::append_integer32_transation(const int32_t value)
{
    int ret = -1;
    do {
        if (!dbus_msg) {
            log_error("need to create transation first");
            break;
        }
        // put string into container
        if(!dbus_message_iter_append_basic(&send_items, DBUS_TYPE_UINT32, &value)) {
            log_error("Append integer32 failed");
            break;
        }
        log_debug("append integer32 %d", value);
        ret = 0;
    } while (false);
    return ret;
}

int DBusClient::append_integer64_transation(const int64_t value)
{
    int ret = -1;
    do {
        if (!dbus_msg) {
            log_error("need to create transation first");
            break;
        }
        // put string into container
        if(!dbus_message_iter_append_basic(&send_items, DBUS_TYPE_UINT64, &value)) {
            log_error("Append integer64 failed");
            break;
        }
        log_debug("append integer64 %lld", value);
        ret = 0;
    } while (false);
    return ret;
}

int DBusClient::commit_transation_with_reply()
{
    int ret = -1;
    DBusPendingCall *pending = NULL;
    dbus_uint32_t *response = NULL;
    do {
        if (!dbus_msg) {
            log_error("need to create transation first");
            break;
        }
        // send it and get blocking flag
        if(!dbus_connection_send_with_reply(conn, dbus_msg, &pending, -1)){
            log_error("Send and reply failed");
            break;
        }
        if (!pending) {
            log_error("Pending is empty, connection is disconnected");
            break;
        }
        dbus_connection_flush(conn);
        // release dbus_msg here, we need it later
        dbus_message_unref(dbus_msg);
        // blocking until receive a reply
        dbus_pending_call_block (pending);
        dbus_msg = dbus_pending_call_steal_reply(pending);
        if (!dbus_msg) {
            log_error("Reply is empty");
            break;
        }
        // free the pending message handle
        dbus_pending_call_unref(pending);
        // read the parameters
        if (!dbus_message_iter_init(dbus_msg, &send_items)) {
            log_error("Empty message");
            ret = DBUS_REPLY_EMPTY;
            // expect all reply are int32
        } else if (dbus_message_iter_get_arg_type (&send_items) != DBUS_TYPE_UINT32) {
            log_error("Reponse is not int");
            ret = DBUS_REPLY_WRONG_TYPE;
        } else {
            dbus_message_iter_get_basic (&send_items, &response);
            ret = (uintptr_t)response;
            //TODO, if response is not 0, what should do?
        }
        log_debug("commit transation with reply");
    } while (false);
    if (dbus_msg) {
        dbus_message_unref(dbus_msg);
        dbus_msg = NULL;
    }
    return ret;
}

int DBusClient::commit_transation_without_reply()
{
    int ret = -1;
    dbus_uint32_t serial = 0;
    dbus_uint32_t *response = NULL;
    do {
        if (!dbus_msg) {
            log_error("need to create transation first");
            break;
        }
        // send it
        if(!dbus_connection_send(conn, dbus_msg, &serial)){
            log_error("Send and reply failed");
            break;
        }
        dbus_connection_flush(conn);
        log_debug("commit transation without reply");
        ret = 0;
    } while (false);
    if (dbus_msg) {
        dbus_message_unref(dbus_msg);
        dbus_msg = NULL;
    }
    return ret;
}
int DBusClient::abort_transation()
{
    int ret = -1;
    do {
        if (!dbus_msg) {
            log_error("need to create transation first");
            break;
        }
        log_debug("about transation");
        //TODO, how to clear up send_items
        ret = 0;
    } while (false);
    if (dbus_msg) {
        dbus_message_unref(dbus_msg);
        dbus_msg = NULL;
    }
    return ret;
}

const char * DBusClient::get_support_type(const DBUS_TYPE_INDEX index)
{
    static const char *DBUS_METHODS[DBUS_TYPE_NUMS] = {
        "Method",
        "Signal",
    };
    return DBUS_METHODS[index];
}

