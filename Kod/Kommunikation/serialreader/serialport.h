/*
 *  MapMaster2001
 *  Erik Ekelund, David Habrman, Tobias Grundström,
 *  Hans-Filip Elo, Niklas Ericsson, Jens Edhammer
 *
 *  TSEA56 2014.
 *
 *  Code produced in part from qt example, which can be found. 
 *  https://qt.gitorious.org/qt/qtserialport/source/c82120dd86881e964bf5104b608932d9e2ec7da4:examples/serialport/creaderasync/serialportreader.cpp
 *
 */

#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QtSerialPort/QSerialPort>
#include <QString>
#include <QTextStream>
#include <QTimer>
#include <QByteArray>
#include <QObject>
#include "../../Styr_m/mapGUI/gui.h"

QT_USE_NAMESPACE
class Gui;


class SerialPort : public QObject
{
    Q_OBJECT

public:
    SerialPort(QSerialPort *serialPort, QObject *parent = 0);
    ~SerialPort();
    void sendArray(const char data[27]);
    QSerialPort *port;
    void setGui(Gui*);
    
private slots:
    void handleReadyRead();
    void handleError(QSerialPort::SerialPortError error);
    void handleData(QByteArray);

private:
    
    void rowReceived();
    Gui* GUI;
    char* QByteToArray(QByteArray);
    QByteArray  m_readData;
    QTextStream m_standardOutput;
    QByteArray tempData;
    QTime test;
};

#endif
