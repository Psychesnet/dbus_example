cmake_minimum_required(VERSION 3.11)
project(DBusClient)

# include path
include_directories(/usr/local/include/dbus-1.0)
include_directories(/usr/local/Cellar/dbus/1.12.16/lib/dbus-1.0/include)

# find extra lib as variable
find_library(USR_LOCAL_LIBRARY
	NAMES dbus-1
	PATH /usr/local/lib
	)

# create a executable program
add_executable(client client.cpp DBusClient.cpp)
add_executable(server server.cpp DBusClient.cpp DBusServer.cpp)
target_link_libraries(client ${LIB_TARGET} ${USR_LOCAL_LIBRARY})
target_link_libraries(server ${LIB_TARGET} ${USR_LOCAL_LIBRARY})

message("Done...")
