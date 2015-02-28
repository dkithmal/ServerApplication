#-------------------------------------------------
#
# Project created by QtCreator 2015-02-17T20:33:43
#
#-------------------------------------------------

QT       += core
QT       += network

QT       -= gui

TARGET = ServerApplication
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    applicationserver.cpp \
    clientcommandhandler.cpp

HEADERS += \
    applicationserver.h \
    clientcommandhandler.h
