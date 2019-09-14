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
    fprintf(stderr, "start listen service\n");
    fprintf(stderr, "host bus name: %s\n", bus_name.c_str());
    fprintf(stderr, "object name: %s\n", object_name.c_str());
    fprintf(stderr, "interface_name: %s\n", interface_name.c_str());
    do {
        //要求监听某个singal：来自接口test.signal.Type的信号
        // dbus_bus_add_match(conn, "interface='signal',interface='test.signal.Type'", &err);
        // dbus_connection_flush(conn);
        // if(dbus_error_is_set(&err)){
            // fprintf(stderr, "Match Error %s\n", err.message);
            // break;
        // }
        while(!exit_flag){
            dbus_connection_read_write(conn, 0);
            msg = dbus_connection_pop_message (conn);
            if(msg == NULL){
                sleep(1);
                continue;
            }
            //TODO, how to handle signal
            if(dbus_message_is_signal(msg,"test.signal.Type","Test")){
                if(!dbus_message_iter_init(msg,&arg)) {
                    fprintf(stderr,"Message Has no Param");
                } else if(dbus_message_iter_get_arg_type(&arg) != DBUS_TYPE_STRING) {
                    fprintf(stderr, "Param is not string");
                } else {
                    dbus_message_iter_get_basic(&arg,&sigvalue);
                    printf("Got Singal with value : %s\n",sigvalue);
                }
            }else if(dbus_message_is_method_call(msg,
                        interface_name.c_str(), HANDLE_METHOD)){
                //我们这里面先比较了接口名字和方法名字，实际上应当现比较路径
                if(strcmp(dbus_message_get_path(msg), object_name.c_str()) == 0) {
                    // TODO, need think about it
                    reply_to_method_call(msg);
                }
            }
            dbus_message_unref(msg);
        }
        ret = 0;
    } while (false);
    return ret;
}

void DBusServer::reply_to_method_call(DBusMessage * msg)
{
    DBusMessage * reply;
    DBusMessageIter arg;
    char * param = NULL;
    dbus_uint32_t ok = DBUS_REPLY_OK;
    dbus_uint32_t serial = 0;

    //从msg中读取参数，这个在上一次学习中学过
    if(!dbus_message_iter_init(msg, &arg)) {
        printf("Message has no args\n");
    } else if(dbus_message_iter_get_arg_type(&arg) != DBUS_TYPE_STRING) {
        printf("Arg is not string!\n");
    } else {
        dbus_message_iter_get_basic(&arg, &param);
    }
    if(param == NULL) return;
    fprintf(stderr, "Get \"%s\"\n", param);

    //创建返回消息reply
    reply = dbus_message_new_method_return(msg);
    dbus_message_iter_init_append(reply, &arg);
    if(!dbus_message_iter_append_basic(&arg, DBUS_TYPE_UINT32, &ok)) {
        printf("Out of Memory!\n");
        exit(1);
    }
    //发送返回消息
    if(!dbus_connection_send(conn, reply, &serial)){
        printf("Out of Memory\n");
        exit(1);
    }
    dbus_connection_flush(conn);
    dbus_message_unref(reply);
}