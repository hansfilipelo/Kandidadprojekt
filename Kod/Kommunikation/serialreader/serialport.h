/*
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

QT_USE_NAMESPACE


class SerialPort : public QObject
{
    Q_OBJECT

public:
    SerialPort(QSerialPort *serialPort, QObject *parent = 0);
    ~SerialPort();
    void sendChar(const char *);
    void sendArray(const char data[25]);

private slots:
    void handleReadyRead();
    void handleError(QSerialPort::SerialPortError error);

private:
    QSerialPort *port;
    QByteArray  m_readData;
    QTextStream m_standardOutput;
    QString tempData;
};

#endif