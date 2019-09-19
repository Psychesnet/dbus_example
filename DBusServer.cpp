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
            fprintf(stderr, "fail to deinit parent function\n");
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
    fprintf(stderr, "start listen service\n");
    fprintf(stderr, "host bus name: %s\n", bus_name.c_str());
    fprintf(stderr, "object name: %s\n", object_name.c_str());
    fprintf(stderr, "interface_name: %s\n", interface_name.c_str());
    fprintf(stderr, "signal bus name: %s\n", BASIC_SIGNAL_NAME);
    fprintf(stderr, "signal interface name %s\n", signal_interface_name.c_str());
    fprintf(stderr, "signal filiter: %s\n", signal_filiter.c_str());
    fprintf(stderr, "signal object name: %s\n", signal_object_name.c_str());
    do {
        // listen signal at interface camera.signal.SERVICE_NAME
        dbus_bus_add_match(conn, signal_filiter.c_str(), &err);
        dbus_connection_flush(conn);
        if(dbus_error_is_set(&err)){
            fprintf(stderr, "Set signal filiter(%s) failed, %s\n",
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
            //TODO, how to handle signal
            if(dbus_message_is_signal(msg, signal_interface_name.c_str(),
                        get_support_type(DBUS_TYPE_SIGNAL))){
                    if(strcmp(dbus_message_get_path(msg), signal_object_name.c_str()) == 0) {
                        parse_and_reply(msg);
                    }
            }else {
                if(dbus_message_is_method_call(msg,
                            interface_name.c_str(), get_support_type(DBUS_TYPE_METHOD))){
                    if(strcmp(dbus_message_get_path(msg), object_name.c_str()) == 0) {
                        // TODO, need think about it
                        parse_and_reply(msg);
                    }
                } // TODO, need to handle others
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
    char * param = NULL;
    dbus_uint32_t ok = DBUS_REPLY_OK;
    dbus_uint32_t serial = 0;
    if (dbus_message_iter_init(msg, &input)) {
        reply = dbus_message_new_method_return(msg);
        dbus_message_iter_init_append(reply, &output);
        while ((current_type = dbus_message_iter_get_arg_type(&input))
                != DBUS_TYPE_INVALID) {
            switch (current_type) {
                case DBUS_TYPE_STRING:
                    dbus_message_iter_get_basic(&input, &param);
                    fprintf(stderr, "Get \"%s\"\n", param);
                    if(!dbus_message_iter_append_basic(&output, DBUS_TYPE_UINT32, &ok)) {
                        printf("Out of Memory!\n");
                    }
                    break;
                default:
                    fprintf(stderr, "type not support(%d)\n", current_type);
                    break;
            }
            dbus_message_iter_next(&input);
        }
        if(!dbus_connection_send(conn, reply, &serial)){
            printf("Out of Memory\n");
        }
        dbus_connection_flush(conn);
        dbus_message_unref(reply);
    } else {
        fprintf(stderr, "message has no args\n");
    }
}

