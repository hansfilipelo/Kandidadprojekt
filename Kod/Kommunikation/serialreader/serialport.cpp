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

/* Write exactly 25 bytes of data to port. Our char arrays are not nullterminated. Will cause issue if not for 25 bytes max.
 * Wait for a maximum och 0.6 seconds for successful transmission.
 */


void SerialPort::sendArray(const char inArray[25]){
    port->write(inArray,25);
    port->waitForBytesWritten(600);
}


/*handleReadyRead(). tempData is the data from potential previous transmission. if the current transmission added with the previous
 * transmission has a length of less than 25 it is saved in tempData and we leave the readyread to accept more data.
 * On 25 or more bytes 25 bytes of data are sent to handleData, excess is stored in tempData for future transmissiondata.
 * This function is not fully tested yet.
 */

void SerialPort::handleReadyRead() {
    
    QByteArray inData;
    
    int inBytes = port->bytesAvailable();
    int tempBytes = tempData.length();
    
    int checkBytes = inBytes + tempBytes;
    
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


/*Function for conversion and deepcopy from QByteArray to char array.
 */

char* SerialPort::QByteToArray(QByteArray inArray)
{
int size = inArray.size();
char *outData = new char(size);
memcpy(outData, inArray.data(), size);
return outData;
}

/*Pointer to gui neccessary for handleData.
 */

void SerialPort::setGui(Gui* ptr){
    GUI = ptr;
}

/*Errorhandling of serialport. Currently only informs. Improvement would be to have it reconnect.
 */

void SerialPort::handleError(QSerialPort::SerialPortError serialPortError)
{
    if (serialPortError == QSerialPort::ReadError) {
        m_standardOutput << QObject::tr("An I/O error occurred while reading the data from port %1, error: %2").arg(port->portName()).arg(port->errorString()) << endl;
        //QCoreApplication::exit(1);
    }
}

/*Passing and preformatting data that arrives from serialport and handleReadyRead.
 */

void SerialPort::handleData(QByteArray inData){
    if(inData[1] == 'M'){
        char* data = QByteToArray(inData);
        GUI->insertRow(data);
        delete data;
    }
    if((int)inData[2] == 31){
        GUI->updateMap();
    }
}
