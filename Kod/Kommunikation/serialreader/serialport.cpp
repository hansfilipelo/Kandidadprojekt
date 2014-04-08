#include "serialport.h"

#include <QCoreApplication>
#include <iostream>

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
    
    port->write(inChar);
    port->waitForBytesWritten(150);
}

void SerialPort::sendArray(const char inArray[25]){
    for(unsigned int i= 0; i < 25; i++){
        sendChar(&inArray[i]);
        
        std::cout << "Sending: " << inArray[i] << std::endl;
    }
}

void SerialPort::handleReadyRead() //untested readyread
{
    QString inData = "";
    inData = port->readAll();
    
    if(inData.length() == 25){
        //handleData(inData);
        inData ="";
        tempData ="";
        return;
    }else if((inData.length() < 25) & (tempData == "")){
        tempData = inData;
        return;
    }else if(inData.length() < 25){
        tempData.append(inData);
        if (tempData.length() < 25){
            return;
        }
        if(tempData.length() == 25){
            //handleData(inData);
            inData = "";
            tempData = "";
            return;
        }
        if(tempData.length() > 25){
            std::cout << "Fatal error too long data from BT" << std::endl;
        }else if(inData.length() > 25){
            std::cout << "Fatal error too long data from BT" << std::endl;
        }
    }
}

void SerialPort::handleError(QSerialPort::SerialPortError serialPortError)
{
    if (serialPortError == QSerialPort::ReadError) {
        m_standardOutput << QObject::tr("An I/O error occurred while reading the data from port %1, error: %2").arg(port->portName()).arg(port->errorString()) << endl;
        QCoreApplication::exit(1);
    }
}