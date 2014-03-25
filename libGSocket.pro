#-------------------------------------------------
#
# Project created by QtCreator 2014-03-24T10:01:45
#
#-------------------------------------------------

QT       += core network

QT       -= gui

QMAKE_CXXFLAGS += -std=gnu++11

TARGET = GSocket
TEMPLATE = lib

DEFINES += LIBGSOCKET_LIBRARY

SOURCES += mgsocket.cpp

HEADERS += mgsocket.h \
    tcpcommands.h \
    mprotocol.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
