#ifndef DAEMONDEFINE_H
#define DAEMONDEFINE_H

// global part
#define BASIC_BUS_NAME      "camera.dbus."
#define BASIC_SIGNAL_NAME   BASIC_BUS_NAME"signal"

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

/*! \enum DBUS_TYPE_KEY
 *
 *  define the key for dbus service
 */
enum DBUS_TYPE_INDEX {
    DBUS_TYPE_METHOD = 0,
    DBUS_TYPE_SIGNAL,
    DBUS_TYPE_NUMS,
};

#endif /* DAEMONDEFINE_H */
