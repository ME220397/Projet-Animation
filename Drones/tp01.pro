#-------------------------------------------------
#
# Project created by QtCreator 2019-01-12T12:27:11
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tp01
TEMPLATE = app


SOURCES += main.cpp\
        drone.cpp \
        dronefactory.cpp \
        jsonreader.cpp \
        princ.cpp \
        glarea.cpp

HEADERS  += princ.h \
        drone.h \
        dronefactory.h \
        glarea.h \
        jsonreader.h

FORMS    += princ.ui

RESOURCES += \
    tp01.qrc
