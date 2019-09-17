#include "DBusClient.h"

DBusClient::DBusClient()
{
    already_init = false;
}

DBusClient::~DBusClient()
{
}

int DBusClient::init(const char *service_name)
{
    int ret = -1;
    do {
        if (already_init) {
            fprintf(stderr, "alreay init service\n");
            break;
        }
        dbus_error_init(&err);
        // set up variables
        bus_name = std::string(BASIC_BUS_NAME) + std::string(service_name);
        // register it
        register_service();
        already_init = true;
        exit_flag = false;
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
            fprintf(stderr, "Get session failed, %s\n", err.message);
            break;
        }
        if (!conn) {
            fprintf(stderr, "Connection is empty\n");
            break;
        }
        // set up unique bus name
        ret = dbus_bus_request_name(conn, bus_name.c_str(), DBUS_NAME_FLAG_REPLACE_EXISTING, &err);
        if (dbus_error_is_set(&err)) {
            fprintf(stderr, "Request bus name failed, %s\n", err.message);
            break;
        }
        if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
            fprintf(stderr, "Not bus name owner\n");
            break;
        }
        ret = 0;
    } while (false);
    return ret;
}

int DBusClient::create_transation(const char *host_name, const DBUS_METHOD_INDEX index)
{
    int ret = -1;
    do {
        std::string target_bus_name = std::string(BASIC_BUS_NAME) + std::string(host_name);
        object_name = "/" + target_bus_name + "/object";
        std::replace(object_name.begin(), object_name.end(), '.', '/');
        interface_name = target_bus_name + ".interface";
		// create a message method call
		// fprintf(stderr, "owner bus name: %s\ntarget bus name: %s\nobj name: %s\ninterface: %s\nkey: %s\nvalue: %s\n",
				// bus_name.c_str(), target_bus_name.c_str(), object_name.c_str(), interface_name.c_str(), key, value);
		dbus_msg = dbus_message_new_method_call(target_bus_name.c_str(), object_name.c_str(),
                interface_name.c_str(), get_support_method(index));
		if (!dbus_msg) {
			fprintf(stderr, "Create message failed, %s\n", err.message);
			break;
		}
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
            fprintf(stderr, "need to create transation first\n");
            break;
        }
		// put string into container
		if(!dbus_message_iter_append_basic(&send_items, DBUS_TYPE_STRING, &value)) {
			fprintf(stderr, "Append string failed\n");
			break;
		}
        ret = 0;
    } while (false);
    return ret;
}

int DBusClient::append_integer32_transation(const int32_t value)
{
    int ret = -1;
    do {
        if (!dbus_msg) {
            fprintf(stderr, "need to create transation first\n");
            break;
        }
		// put string into container
		if(!dbus_message_iter_append_basic(&send_items, DBUS_TYPE_UINT32, &value)) {
			fprintf(stderr, "Append string failed\n");
			break;
		}
        ret = 0;
    } while (false);
    return ret;
}

int DBusClient::append_integer64_transation(const int64_t value)
{
    int ret = -1;
    do {
        if (!dbus_msg) {
            fprintf(stderr, "need to create transation first\n");
            break;
        }
		// put string into container
		if(!dbus_message_iter_append_basic(&send_items, DBUS_TYPE_UINT64, &value)) {
			fprintf(stderr, "Append string failed\n");
			break;
		}
        ret = 0;
    } while (false);
    return ret;
}

int DBusClient::commit_transation()
{
    int ret = -1;
	DBusPendingCall *pending = NULL;
	dbus_uint32_t *response = NULL;
    do {
        if (!dbus_msg) {
            fprintf(stderr, "need to create transation first\n");
            break;
        }
		// send it and get blocking flag
		if(!dbus_connection_send_with_reply(conn, dbus_msg, &pending, -1)){
			fprintf(stderr, "Send and reply failed\n");
			break;
		}
		if (!pending) {
			fprintf(stderr, "Pending is empty, connection is disconnected\n");
			break;
		}
		dbus_connection_flush(conn);
		// release dbus_msg here, we need it later
		dbus_message_unref(dbus_msg);
		// blocking until receive a reply
		dbus_pending_call_block (pending);
		dbus_msg = dbus_pending_call_steal_reply(pending);
		if (!dbus_msg) {
			fprintf(stderr, "Reply is empty\n");
			break;
		}
		// free the pending message handle
		dbus_pending_call_unref(pending);
		// read the parameters
		if (!dbus_message_iter_init(dbus_msg, &send_items)) {
			fprintf(stderr, "Empty message\n");
            ret = DBUS_REPLY_EMPTY;
        // expect all reply are int32
		} else if (dbus_message_iter_get_arg_type (&send_items) != DBUS_TYPE_UINT32) {
			fprintf(stderr, "Reponse is not int\n");
            ret = DBUS_REPLY_WRONG_TYPE;
		} else {
			dbus_message_iter_get_basic (&send_items, &response);
		    ret = (uintptr_t)response;
            //TODO, if response is not 0, what should do?
		}
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
            fprintf(stderr, "need to create transation first\n");
            break;
        }
        //TODO, how to clear up send_items
        ret = 0;
	} while (false);
	if (dbus_msg) {
		dbus_message_unref(dbus_msg);
        dbus_msg = NULL;
	}
    return ret;
}

const char * DBusClient::get_support_method(const DBUS_METHOD_INDEX index)
{
    static const char *DBUS_METHODS[] = {
        "Method",
        "Signal",
        "AskDaemon",
    };
    return DBUS_METHODS[index];
}

