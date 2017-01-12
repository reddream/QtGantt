#-------------------------------------------------
#
# Project created by QtCreator 2017-01-10T14:53:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gantt-demo
TEMPLATE = app

include(../gantt-lib-include.pri)
include(../demo-lib-include.pri)

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

