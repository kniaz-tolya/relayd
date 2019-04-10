QT += core network
QT -= gui

CONFIG += c++11

TARGET = relayd
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  main.cpp \
    server.cpp \
    worker.cpp \
    app.cpp

HEADERS +=  \
    server.h \
    worker.h \
    app.h
