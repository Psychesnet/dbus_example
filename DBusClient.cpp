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

// basic functions
int DBusClient::send_string(
        const char* host_name,
        const char *key,
        const char *value)
{
    int ret = -1;
	DBusMessage *msg = NULL;
	DBusMessageIter items;
	DBusPendingCall *pending = NULL;
	dbus_uint32_t *response = NULL;
    do {
        std::string target_bus_name = std::string(BASIC_BUS_NAME) + std::string(host_name);
        object_name = "/" + target_bus_name + "/object";
        std::replace(object_name.begin(), object_name.end(), '.', '/');
        interface_name = target_bus_name + ".interface";
		// create a message method call
		fprintf(stderr, "owner bus name: %s\ntarget bus name: %s\nobj name: %s\ninterface: %s\nkey: %s\nvalue: %s\n",
				bus_name.c_str(), target_bus_name.c_str(), object_name.c_str(), interface_name.c_str(), key, value);
		msg = dbus_message_new_method_call(target_bus_name.c_str(), object_name.c_str(),
                interface_name.c_str(), key);
		if (!msg) {
			fprintf(stderr, "Create message failed, %s\n", err.message);
			break;
		}
		// init container and link to msg
		dbus_message_iter_init_append(msg, &items);
		// put string into container
		if(!dbus_message_iter_append_basic(&items, DBUS_TYPE_STRING, &value)) {
			fprintf(stderr, "Append string failed\n");
			break;
		}
		// send it and get blocking flag
		if(!dbus_connection_send_with_reply(conn, msg, &pending, -1)){
			fprintf(stderr, "Send and reply failed\n");
			break;
		}
		if (!pending) {
			fprintf(stderr, "Pending is empty, connection is disconnected\n");
			break;
		}
		dbus_connection_flush(conn);
		// release msg here, we need it later
		dbus_message_unref(msg);
		// blocking until receive a reply
		dbus_pending_call_block (pending);
		msg = dbus_pending_call_steal_reply(pending);
		if (!msg) {
			fprintf(stderr, "Reply is empty\n");
			break;
		}
		// free the pending message handle
		dbus_pending_call_unref(pending);
		// read the parameters
		if (!dbus_message_iter_init(msg, &items)) {
			fprintf(stderr, "Empty message\n");
            ret = DBUS_REPLY_EMPTY;
		} else if (dbus_message_iter_get_arg_type (&items) != DBUS_TYPE_UINT32) {
			fprintf(stderr, "Reponse is not int\n");
            ret = DBUS_REPLY_WRONG_TYPE;
		} else {
			dbus_message_iter_get_basic (&items, &response);
		    ret = (uintptr_t)response;
            //TODO, if response is not 0, what should do?
		}
	} while (false);
	if (msg) {
		dbus_message_unref(msg);
	}
    return ret;
}
