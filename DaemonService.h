#ifndef DAEMONDEFINE_H
#define DAEMONDEFINE_H

// global part
#define BASIC_BUS_NAME "camera.dbus."

// daemon part
#define DAEMON_PRAWN        "prawn"
#define DAEMON_SHEEP        "sheep"
#define DAEMON_CROCODILE    "crocodile"

/*! \enum feedback
 *
 *  Detailed description
 */
enum {
    DBUS_REPLY_OK = 0,
    DBUS_REPLY_EMPTY = 1,
    DBUS_REPLY_WRONG_TYPE = 2,
};

/*! \enum DBUS_METHOD_KEY
 *
 *  define the key for dbus service
 */
enum DBUS_METHOD_INDEX {
    DBUS_INDEX_METHOD = 0,
    DBUS_INDEX_SIGNAL,
    DBUS_INDEX_ASK_DAEMON,
    DBUS_INDEX_NUMS,
};

#endif /* DAEMONDEFINE_H */
