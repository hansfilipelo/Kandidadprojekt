/****************************************************************************
 **
 ** Copyright (C) 2013 Laszlo Papp <lpapp@kde.org>
 ** Contact: http://www.qt-project.org/legal
 **
 ** This file is part of the QtSerialPort module of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:LGPL$
 ** Commercial License Usage
 ** Licensees holding valid commercial Qt licenses may use this file in
 ** accordance with the commercial license agreement provided with the
 ** Software or, alternatively, in accordance with the terms contained in
 ** a written agreement between you and Digia.  For licensing terms and
 ** conditions see http://qt.digia.com/licensing.  For further information
 ** use the contact form at http://qt.digia.com/contact-us.
 **
 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Digia gives you certain additional
 ** rights.  These rights are described in the Digia Qt LGPL Exception
 ** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
 **
 ** GNU General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU
 ** General Public License version 3.0 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU General Public License version 3.0 requirements will be
 ** met: http://www.gnu.org/copyleft/gpl.html.
 **
 **
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

#include "serialportreader.h"

#include <QCoreApplication>

QT_USE_NAMESPACE

SerialPortReader::SerialPortReader(QSerialPort *serialPort, QObject *parent)
: QObject(parent)
, m_serialPort(serialPort)
, m_standardOutput(stdout)
{
    connect(m_serialPort, SIGNAL(readyRead()), SLOT(handleReadyRead()));
    connect(m_serialPort, SIGNAL(error(QSerialPort::SerialPortError)), SLOT(handleError(QSerialPort::SerialPortError)));
}

SerialPortReader::~SerialPortReader()
{
}
/*
 Create protocol with startsign, DATA, stopsign.
 If the stopsign hasn't arrived in the readyread, save data in the unfinished
 data collection variable.
 If the stopsign has arrived, append unfinished data with this data and move
 data to finished data for processing.
 Computer should be faster on processing than AVR can send to us.
 Will there be conflicts if sending to AVR while data is coming to processor.
 */
void SerialPortReader::handleReadyRead()
{
	m_readData = "";
    if (temporary_readData != "")
	{
    	m_readData = temporary_readData;
		temporary_readData = "";
    }
    
    QByteArray data = m_serialPort->read(24);
    QString commandName = data.left(2);
    QByteArray inData = data.mid(1);
    
    // Check which command that's supposed to run
    if (commandName == "MP"){
        handleMapData(inData);
    }
    else if(commandName =="GY"){
        handleGyroData(inData);
    }
    else if(commandName =="RF"){
        handleRFIDData(inData);
    }
    else if(commandName.left(1) =="L"){
        handleLongRangeData(inData,commandName.right(1).toInt());
    }
    else if(commandName.left(1) =="S"){
        handleShortRangeData(inData,commandName.right(1).toInt());
    }
    else if(commandName.left(1) =="M"){
        handleMotorData(inData,commandName.right(1).toInt());
    }
    else {
        qDebug() << "Unknown command: ";
        return;
    }
}

void SerialPortReader::printData(QByteArray data)
{
	m_standardOutput << data << endl;
}

void SerialPortReader::handleMapData(QByteArray inData)
{
    qDebug() << "Handeling map!";
    this->printData(inData);
}
void SerialPortReader::handleGyroData(QByteArray inData)
{

}
void SerialPortReader::handleRFIDData(QByteArray inData)
{

}
void SerialPortReader::handleShortRangeData(QByteArray inData, unsigned int nr)
{

}
void SerialPortReader::handleLongRangeData(QByteArray inData, unsigned int nr)
{

}
void SerialPortReader::handleMotorData(QByteArray inData, unsigned int nr)
{

}



void SerialPortReader::handleError(QSerialPort::SerialPortError serialPortError)
{
    if (serialPortError == QSerialPort::ReadError) {
        m_standardOutput << QObject::tr("An I/O error occurred while reading the data from port %1, error: %2").arg(m_serialPort->portName()).arg(m_serialPort->errorString()) << endl;
        QCoreApplication::exit(1);
    }
}