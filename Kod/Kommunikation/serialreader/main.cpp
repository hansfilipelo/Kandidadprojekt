#include "serialport.h"
#include "order.h"

#include <QtSerialPort/QSerialPort>
#include <QtCore>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <QString>
#include <QTextStream>
#include <QCoreApplication>
#include <QFile>
#include <QStringList>

QT_USE_NAMESPACE

int main(int argc, char *argv[])
{
    QCoreApplication coreApplication(argc, argv);
    int argumentCount = QCoreApplication::arguments().size();
    QStringList argumentList = QCoreApplication::arguments();
    
    QTextStream standardOutput(stdout);
    QTextStream standardInput(stdin);
    
    if (argumentCount == 1) {
        standardOutput << QObject::tr("Usage: %1 <serialportname> [baudrate]").arg(argumentList.first()) << endl;
        return 1;
    }
    
    
    QSerialPort serialPort;
    QString serialPortName = argumentList.at(1);
    serialPort.setPortName(serialPortName);
    
    if (!serialPort.open(QIODevice::ReadWrite)) {
        standardOutput << QObject::tr("Failed to open port %1, error: %2").arg(serialPortName).arg(serialPort.errorString()) << endl;
        return 1;
    }
    
    int serialPortBaudRate = (argumentCount > 2) ? argumentList.at(2).toInt() : QSerialPort::Baud9600;
    if (!serialPort.setBaudRate(serialPortBaudRate)) {
        standardOutput << QObject::tr("Failed to set 9600 baud for port %1, error: %2").arg(serialPortName).arg(serialPort.errorString()) << endl;
        return 1;
    }
    
    if (!serialPort.setDataBits(QSerialPort::Data8)) {
        standardOutput << QObject::tr("Failed to set 8 data bits for port %1, error: %2").arg(serialPortName).arg(serialPort.errorString()) << endl;
        return 1;
    }
    
    if (!serialPort.setParity(QSerialPort::NoParity)) {
        standardOutput << QObject::tr("Failed to set no parity for port %1, error: %2").arg(serialPortName).arg(serialPort.errorString()) << endl;
        return 1;
    }
    
    if (!serialPort.setStopBits(QSerialPort::OneStop)) {
        standardOutput << QObject::tr("Failed to set 1 stop bit for port %1, error: %2").arg(serialPortName).arg(serialPort.errorString()) << endl;
        return 1;
    }
    
    if (!serialPort.setFlowControl(QSerialPort::NoFlowControl)) {
        standardOutput << QObject::tr("Failed to set no flow control for port %1, error: %2").arg(serialPortName).arg(serialPort.errorString()) << endl;
        return 1;
    }
    
    SerialPort port(&serialPort);
    Order order(&port);
    
    order.test();
    
    return coreApplication.exec();

}