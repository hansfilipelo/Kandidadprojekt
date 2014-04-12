#-------------------------------------------------
#
# Project created by QtCreator 2014-04-03T10:57:12
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = mapGUI
TEMPLATE = app

CONFIG		+= c++11

SOURCES += main.cpp \
    gui.cpp \
    qcustomplot.cpp \
    ../../../Kommunikation/serialreader/order.cpp \
    ../../../Kommunikation/serialreader/serialport.cpp

HEADERS  += \
    gui.h \
    qcustomplot.h \
    ../../../Kommunikation/serialreader/serialport.h \
    ../../../Kommunikation/serialreader/order.h

FORMS    += \
    gui.ui
