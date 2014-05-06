#-------------------------------------------------
#
# Project created by QtCreator 2014-05-06T17:07:05
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = graphGui
TEMPLATE = app

CONFIG		+= c++11


SOURCES += main.cpp\
        mainwindow.cpp \
    ../Styr_m/Styr_m/mapGUI/qcustomplot.cpp

HEADERS  += mainwindow.h \
    ../Styr_m/Styr_m/mapGUI/qcustomplot.h

FORMS    += mainwindow.ui
