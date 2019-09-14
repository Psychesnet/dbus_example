#ifndef DAEMONDEFINE_H
#define DAEMONDEFINE_H

// global part
#define BASIC_BUS_NAME "camera.dbus."

// daemon part
#define DAEMON_SHEEP        "sheep"
#define DAEMON_CROCODILE    "crocodile"

// key(method) define
#define  HANDLE_METHOD "Method"
#define  HANDLE_SIGNAL "Signal"

/*! \enum feedback
 *
 *  Detailed description
 */
enum {
    DBUS_REPLY_OK = 0,
    DBUS_REPLY_EMPTY = 1,
    DBUS_REPLY_WRONG_TYPE = 2,
};

#endif /* DAEMONDEFINE_H */
