cmake_minimum_required(VERSION 3.11)
include(FindPkgConfig)

project(DBusClient)

find_package(PkgConfig REQUIRED)
set(CMAKE_PREFIX_PATH "/usr/local/Cellar/dbus/1.12.12/lib/pkgconfig")
if(PKG_CONFIG_FOUND)
    pkg_search_module(dbus REQUIRED dbus-1)
    include_directories(${dbus_INCLUDE_DIRS})
    link_directories(${dbus_LIBRARIES})
    add_compile_options(${dbus_CFLAGS_OTHER})
    set(ALL_LIBS ${ALL_LIBS} ${dbus_LIBRARIES})
endif()

add_executable(client client.cpp DBusClient.cpp)
add_executable(server server.cpp DBusClient.cpp DBusServer.cpp)
target_link_libraries(client ${ALL_LIBS})
target_link_libraries(server ${ALL_LIBS})

message("Done...")
