#include "DBusServer.h"

DBusServer::DBusServer()
{
}

DBusServer::~DBusServer()
{
}

int DBusServer::deinit()
{
    int ret = -1;
    do {
        if (DBusClient::deinit() != 0) {
            log_error("fail to deinit parent function");
            break;
        }
        stop_listen_thread();
        ret = 0;
    } while (false);
    return ret;
}

int DBusServer::start_listen_thread()
{
    return pthread_create(&pid, NULL, start_listen_service, this);
}

int DBusServer::stop_listen_thread()
{
    pthread_join(pid, NULL);
    return 0;
}

void * DBusServer::start_listen_service(void *args)
{
    DBusServer *server = (DBusServer *)args;
    server->listen_loop();
    pthread_exit(NULL);
}

int DBusServer::listen_loop()
{
    int ret = -1;
    DBusMessage * msg;
    DBusMessageIter arg;
    char *sigvalue;
    object_name = "/" + std::string(bus_name) + "/object";
    std::replace(object_name.begin(), object_name.end(), '.', '/');
    interface_name = std::string(bus_name) + ".interface";
    std::string signal_bus_name = std::string(BASIC_SIGNAL_NAME);
    std::string signal_interface_name = signal_bus_name + ".interface";
    std::string signal_filiter = std::string("type='signal',interface='") +
        signal_interface_name + "'";
    std::string signal_object_name = "/" + signal_bus_name + "/object";
    std::replace(signal_object_name.begin(), signal_object_name.end(), '.', '/');
    log_info("start listen service");
    log_debug("host bus name: %s", bus_name.c_str());
    log_debug("object name: %s", object_name.c_str());
    log_debug("interface_name: %s", interface_name.c_str());
    log_debug("signal bus name: %s", BASIC_SIGNAL_NAME);
    log_debug("signal interface name %s", signal_interface_name.c_str());
    log_debug("signal filiter: %s", signal_filiter.c_str());
    log_debug("signal object name: %s", signal_object_name.c_str());
    do {
        // listen signal at interface camera.signal.SERVICE_NAME
        dbus_bus_add_match(conn, signal_filiter.c_str(), &err);
        dbus_connection_flush(conn);
        if(dbus_error_is_set(&err)){
            log_error("Set signal filiter(%s) failed, %s",
                    signal_filiter.c_str(), err.message);
            break;
        }
        while(!exit_flag){
            dbus_connection_read_write(conn, 0);
            msg = dbus_connection_pop_message (conn);
            if(msg == NULL){
                sleep(1);
                continue;
            }
            if(dbus_message_is_signal(msg, signal_interface_name.c_str(),
                        get_support_type(DBUS_TYPE_SIGNAL))){
                if(strcmp(dbus_message_get_path(msg), signal_object_name.c_str()) == 0) {
                    parse_and_noreply(msg);
                }
            }else if(dbus_message_is_method_call(msg, interface_name.c_str(),
                        get_support_type(DBUS_TYPE_METHOD))){
                if(strcmp(dbus_message_get_path(msg), object_name.c_str()) == 0) {
                    parse_and_reply(msg);
                }
            }
            dbus_message_unref(msg);
        }
        ret = 0;
    } while (false);
    return ret;
}

void DBusServer::parse_and_reply(DBusMessage *msg)
{
    int current_type;
    DBusMessage *reply;
    DBusMessageIter input, output;
    dbus_uint32_t ok = DBUS_REPLY_OK;
    dbus_uint32_t serial = 0;
    if (dbus_message_iter_init(msg, &input)) {
        reply = dbus_message_new_method_return(msg);
        dbus_message_iter_init_append(reply, &output);
        while ((current_type = dbus_message_iter_get_arg_type(&input))
                != DBUS_TYPE_INVALID) {
            switch (current_type) {
                case DBUS_TYPE_STRING:
                    {
                        char * param = NULL;
                        dbus_message_iter_get_basic(&input, &param);
                        ok = handle_string(param);
                    }
                    break;
                case DBUS_TYPE_UINT32:
                    {
                        uint32_t param = 0;
                        dbus_message_iter_get_basic(&input, &param);
                        ok = handle_uint32(param);
                    }
                    break;
                case DBUS_TYPE_UINT64:
                    {
                        uint64_t param = 0;
                        dbus_message_iter_get_basic(&input, &param);
                        ok = handle_uint64(param);
                    }
                    break;
                default:
                    log_error("type not support(%d)", current_type);
                    ok = DBUS_REPLY_WRONG_TYPE;
                    break;
            }
            if(!dbus_message_iter_append_basic(&output, DBUS_TYPE_UINT32, &ok)) {
                log_warning("prepare reponse failed");
            }
            dbus_message_iter_next(&input);
        }
        if(!dbus_connection_send(conn, reply, &serial)){
            log_warning("send response failed");
        }
        dbus_connection_flush(conn);
        dbus_message_unref(reply);
    } else {
        log_warning("message has no args");
    }
}

void DBusServer::parse_and_noreply(DBusMessage *msg)
{
    int current_type;
    DBusMessageIter input;
    if (dbus_message_iter_init(msg, &input)) {
        while ((current_type = dbus_message_iter_get_arg_type(&input))
                != DBUS_TYPE_INVALID) {
            switch (current_type) {
                case DBUS_TYPE_STRING:
                    {
                        char * param = NULL;
                        dbus_message_iter_get_basic(&input, &param);
                        handle_string(param);
                    }
                    break;
                case DBUS_TYPE_UINT32:
                    {
                        uint32_t param = 0;
                        dbus_message_iter_get_basic(&input, &param);
                        handle_uint32(param);
                    }
                    break;
                case DBUS_TYPE_UINT64:
                    {
                        uint64_t param = 0;
                        dbus_message_iter_get_basic(&input, &param);
                        handle_uint64(param);
                    }
                    break;
                default:
                    log_error("type not support(%d)", current_type);
                    break;
            }
            dbus_message_iter_next(&input);
        }
        dbus_connection_flush(conn);
    } else {
        log_warning("message has no args");
    }
}

uint32_t DBusServer::handle_string(const char * param)
{
    uint32_t ret = DBUS_REPLY_OK;
    log_info("%s %s", __func__, param);
    return ret;
}

uint32_t DBusServer::handle_uint32(const uint32_t param)
{
    uint32_t ret = DBUS_REPLY_OK;
    log_info("%s %u", __func__, param);
    return ret;
}

uint32_t DBusServer::handle_uint64(const uint64_t param)
{
    uint32_t ret = DBUS_REPLY_OK;
    log_info("%s %llu", __func__, param);
    return ret;
}
