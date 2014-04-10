#-------------------------------------------------
#
# Project created by QtCreator 2014-04-03T10:57:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = mapGUI
TEMPLATE = app

CONFIG		+= c++11

SOURCES += main.cpp \
    gui.cpp \
    qcustomplot.cpp \
    order.cpp \
    serialport.cpp

HEADERS  += \
    gui.h \
    qcustomplot.h \
    serialport.h \
    order.h

FORMS    += \
    gui.ui
