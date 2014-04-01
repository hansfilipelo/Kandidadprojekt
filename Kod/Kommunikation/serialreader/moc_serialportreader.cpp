/****************************************************************************
** Meta object code from reading C++ file 'serialportreader.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "serialportreader.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'serialportreader.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_SerialPortReader_t {
    QByteArrayData data[12];
    char stringdata[183];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_SerialPortReader_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_SerialPortReader_t qt_meta_stringdata_SerialPortReader = {
    {
QT_MOC_LITERAL(0, 0, 16),
QT_MOC_LITERAL(1, 17, 15),
QT_MOC_LITERAL(2, 33, 0),
QT_MOC_LITERAL(3, 34, 11),
QT_MOC_LITERAL(4, 46, 28),
QT_MOC_LITERAL(5, 75, 5),
QT_MOC_LITERAL(6, 81, 13),
QT_MOC_LITERAL(7, 95, 14),
QT_MOC_LITERAL(8, 110, 14),
QT_MOC_LITERAL(9, 125, 19),
QT_MOC_LITERAL(10, 145, 20),
QT_MOC_LITERAL(11, 166, 15)
    },
    "SerialPortReader\0handleReadyRead\0\0"
    "handleError\0QSerialPort::SerialPortError\0"
    "error\0handleMapData\0handleRFIDData\0"
    "handleGyroData\0handleLongRangeData\0"
    "handleShortRangeData\0handleMotorData\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SerialPortReader[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x08,
       3,    1,   55,    2, 0x08,
       6,    1,   58,    2, 0x08,
       7,    1,   61,    2, 0x08,
       8,    1,   64,    2, 0x08,
       9,    2,   67,    2, 0x08,
      10,    2,   72,    2, 0x08,
      11,    2,   77,    2, 0x08,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, QMetaType::QByteArray,    2,
    QMetaType::Void, QMetaType::QByteArray,    2,
    QMetaType::Void, QMetaType::QByteArray,    2,
    QMetaType::Void, QMetaType::QByteArray, QMetaType::UInt,    2,    2,
    QMetaType::Void, QMetaType::QByteArray, QMetaType::UInt,    2,    2,
    QMetaType::Void, QMetaType::QByteArray, QMetaType::UInt,    2,    2,

       0        // eod
};

void SerialPortReader::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SerialPortReader *_t = static_cast<SerialPortReader *>(_o);
        switch (_id) {
        case 0: _t->handleReadyRead(); break;
        case 1: _t->handleError((*reinterpret_cast< QSerialPort::SerialPortError(*)>(_a[1]))); break;
        case 2: _t->handleMapData((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 3: _t->handleRFIDData((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 4: _t->handleGyroData((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 5: _t->handleLongRangeData((*reinterpret_cast< const QByteArray(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2]))); break;
        case 6: _t->handleShortRangeData((*reinterpret_cast< const QByteArray(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2]))); break;
        case 7: _t->handleMotorData((*reinterpret_cast< const QByteArray(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject SerialPortReader::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SerialPortReader.data,
      qt_meta_data_SerialPortReader,  qt_static_metacall, 0, 0}
};


const QMetaObject *SerialPortReader::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SerialPortReader::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SerialPortReader.stringdata))
        return static_cast<void*>(const_cast< SerialPortReader*>(this));
    return QObject::qt_metacast(_clname);
}

int SerialPortReader::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
