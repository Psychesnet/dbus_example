cmake_minimum_required(VERSION 3.11)
include(FindPkgConfig)
project(DBusClient)

set(TARS
    client
    server
    dbus-example
)
set(SRCS
    DBusClient.cpp
    DBusServer.cpp
    LoggerInterface.cpp
    LoggerStdout.cpp
    LoggerStderr.cpp
    LoggerSyslog.cpp
    Logger.cpp
    Backtrace.cpp
)
set(DEP_LIBS dbus-1)
set(EXTRA_PKG_PATHS "/usr/lib/pkgconfig" "/usr/local/Cellar/dbus/1.12.12/lib/pkgconfig")

# short __FILE__ path
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-builtin-macro-redefined -D'__FILE__=\"$(subst $(realpath ${CMAKE_SOURCE_DIR})/,,$(abspath $<))\"'")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-builtin-macro-redefined -D'__FILE__=\"$(subst $(realpath ${CMAKE_SOURCE_DIR})/,,$(abspath $<))\"'")

# select os parameters
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D'${CMAKE_HOST_SYSTEM_NAME}'")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D'${CMAKE_HOST_SYSTEM_NAME}'")

find_package(PkgConfig REQUIRED)
if(PKG_CONFIG_FOUND)
    foreach(item ${EXTRA_PKG_PATHS})
        set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${item})
        message("Set extra pkg path +${item}")
    endforeach()

    foreach(item ${DEP_LIBS})
        pkg_search_module(${item} REQUIRED ${item})
        message("Import library link and header +${item}")
        include_directories(${${item}_INCLUDE_DIRS})
        link_directories(${${item}_LIBRARY_DIRS})
        add_compile_options(${${item}_CFLAGS_OTHER})
        set(ALL_LIBS ${ALL_LIBS} ${${item}_LIBRARIES})
    endforeach()

    foreach(item ${TARS})
        message("Prepare to build +${item}")
        add_executable(${item} ${item}.cpp ${SRCS})
        target_link_libraries(${item} ${ALL_LIBS})
    endforeach()
else()
    message("Host system did not support pkg-config, abort!!!")
endif()


