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

void SerialPort::sendArray(const char inArray[25]){
    port->write(inArray,25);                           //write exactly 25 bytes of data to port.
    port->waitForBytesWritten(600);
}

/*
//Alt. handleReadyRead(), assuming tempData is QByteArray
void SerialPort::handleReadyRead() {
    QByteArray inData;
    int inBytes = port->bytesAvailable();
    int tempBytes = tempData.length();
    
    checkBytes = inBytes + tempBytes;
    
    if(checkBytes == 25){
        inData = port->readAll();
        tempData.append(inData);
        handleData(tempData);
        inData.clear();
        tempData.clear();
        return;
    }
    if(checkBytes < 25){                //too little data to handle, save data and leave rdyread.
        inData = port->readAll();
        tempData.append(inData);
        inData.clear();
        return;
    }
    if(checkBytes >25){
        inData = port->read(25-tempBytes); //Read bytes to fill to exact 25 bytes
        tempData.append(inData);           //put data together.
        handleData(tempData);               // handle the data and clear the variables.
        tempData.clear();
        inData.clear();
        return;
 
    }
 }
 */



void SerialPort::handleReadyRead() //semitested readyread will fail to handle if alot of data arrives fast with no spaces inbetween.
{
    QString inData = "";
    inData = port->readAll();
    m_standardOutput << inData << endl;
    if(inData.length() == 25){
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
    }
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
