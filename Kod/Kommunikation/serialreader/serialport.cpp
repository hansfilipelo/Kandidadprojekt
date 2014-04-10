#include "serialport.h"

#include <QCoreApplication>
#include <iostream>
#include <QDebug>
#include <QChar>

QT_USE_NAMESPACE

SerialPort::SerialPort(QSerialPort *serialPort, QObject *parent)
: QObject(parent)
, port(serialPort)
, m_standardOutput(stdout)
{
    connect(port, SIGNAL(readyRead()), SLOT(handleReadyRead()));
    connect(port, SIGNAL(error(QSerialPort::SerialPortError)), SLOT(handleError(QSerialPort::SerialPortError)));
}

SerialPort::~SerialPort()
{
}

void SerialPort::sendChar(const char * inChar){
    
    std::cout << sizeof(inChar) << std::endl;
    port->write(inChar);
    port->waitForBytesWritten(1000);
}

void SerialPort::sendArray(const char inArray[25]){
    port->write(inArray);

}

void SerialPort::handleReadyRead() //untested readyread
{
    QString inData = "";
    inData = port->readAll();
    m_standardOutput << inData << endl;
    /*if(inData.length() == 25){
        handleData(inData);
        inData ="";
        tempData ="";
        std::cout << "first" << std::endl;
        return;
    }else if((inData.length() < 25) & (tempData == "")){
        tempData = inData;
        std::cout << "second. " << inData.length()  << std::endl;
        return;
    }else if(inData.length() < 25){
        std::cout << "third" << inData.length() << std::endl;
        tempData.append(inData);
        if (tempData.length() < 25){
            std::cout << "fourth" << tempData.length() << std::endl;
            return;
        }
        if(tempData.length() == 25){
            std::cout << "fifth" << std::endl;
            handleData(tempData);
            inData = "";
            tempData = "";
            return;
        }
        if(tempData.length() > 25){
            std::cout << "Fatal error too long data from BT" << std::endl;
        }else if(inData.length() > 25){
            std::cout << "Fatal error too long data from BT" << std::endl;
        }
    }*/
}

void SerialPort::handleError(QSerialPort::SerialPortError serialPortError)
{
    if (serialPortError == QSerialPort::ReadError) {
        m_standardOutput << QObject::tr("An I/O error occurred while reading the data from port %1, error: %2").arg(port->portName()).arg(port->errorString()) << endl;
        QCoreApplication::exit(1);
    }
}

void SerialPort::handleData(QString inData){
    unsigned int test = inData.toStdString().at(0);
        std::cout << test << inData.toStdString() << std::endl;
    return;
    
}