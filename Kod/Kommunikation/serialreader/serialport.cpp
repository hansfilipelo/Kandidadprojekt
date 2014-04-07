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
    char temp = *inChar;
    
    std::cout << static_cast<int>(temp) << std::endl;
}

void SerialPort::sendArray(const char inArray[25]){
    for(unsigned int i= 0; i < 25; i++){
        sendChar(&inArray[i]);
    }
}

void SerialPort::handleReadyRead()
{
   
}

void SerialPort::handleError(QSerialPort::SerialPortError serialPortError)
{
    if (serialPortError == QSerialPort::ReadError) {
        m_standardOutput << QObject::tr("An I/O error occurred while reading the data from port %1, error: %2").arg(port->portName()).arg(port->errorString()) << endl;
        QCoreApplication::exit(1);
    }
}