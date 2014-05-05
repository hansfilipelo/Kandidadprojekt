/****************************************************************************
** Meta object code from reading C++ file 'gui.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "gui.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Gui_t {
    QByteArrayData data[19];
    char stringdata[419];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_Gui_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_Gui_t qt_meta_stringdata_Gui = {
    {
QT_MOC_LITERAL(0, 0, 3),
QT_MOC_LITERAL(1, 4, 27),
QT_MOC_LITERAL(2, 32, 0),
QT_MOC_LITERAL(3, 33, 5),
QT_MOC_LITERAL(4, 39, 29),
QT_MOC_LITERAL(5, 69, 16),
QT_MOC_LITERAL(6, 86, 26),
QT_MOC_LITERAL(7, 113, 23),
QT_MOC_LITERAL(8, 137, 23),
QT_MOC_LITERAL(9, 161, 24),
QT_MOC_LITERAL(10, 186, 26),
QT_MOC_LITERAL(11, 213, 27),
QT_MOC_LITERAL(12, 241, 21),
QT_MOC_LITERAL(13, 263, 21),
QT_MOC_LITERAL(14, 285, 23),
QT_MOC_LITERAL(15, 309, 22),
QT_MOC_LITERAL(16, 332, 29),
QT_MOC_LITERAL(17, 362, 23),
QT_MOC_LITERAL(18, 386, 31)
    },
    "Gui\0on_speedSlider_valueChanged\0\0value\0"
    "on_speedSlider_sliderReleased\0"
    "updateTimeVector\0on_actionForward_triggered\0"
    "on_actionDown_triggered\0on_actionLeft_triggered\0"
    "on_actionRight_triggered\0"
    "on_actionSpeedUp_triggered\0"
    "on_actionSlowDown_triggered\0"
    "on_pushButton_clicked\0on_stopButton_pressed\0"
    "on_actionStop_triggered\0on_fetchButton_pressed\0"
    "on_setParameterButton_pressed\0"
    "on_temp90Button_clicked\0"
    "on_actionSetParameter_triggered\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Gui[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   94,    2, 0x08,
       4,    0,   97,    2, 0x08,
       5,    0,   98,    2, 0x08,
       6,    0,   99,    2, 0x08,
       7,    0,  100,    2, 0x08,
       8,    0,  101,    2, 0x08,
       9,    0,  102,    2, 0x08,
      10,    0,  103,    2, 0x08,
      11,    0,  104,    2, 0x08,
      12,    0,  105,    2, 0x08,
      13,    0,  106,    2, 0x08,
      14,    0,  107,    2, 0x08,
      15,    0,  108,    2, 0x08,
      16,    0,  109,    2, 0x08,
      17,    0,  110,    2, 0x08,
      18,    0,  111,    2, 0x08,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Gui::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Gui *_t = static_cast<Gui *>(_o);
        switch (_id) {
        case 0: _t->on_speedSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->on_speedSlider_sliderReleased(); break;
        case 2: _t->updateTimeVector(); break;
        case 3: _t->on_actionForward_triggered(); break;
        case 4: _t->on_actionDown_triggered(); break;
        case 5: _t->on_actionLeft_triggered(); break;
        case 6: _t->on_actionRight_triggered(); break;
        case 7: _t->on_actionSpeedUp_triggered(); break;
        case 8: _t->on_actionSlowDown_triggered(); break;
        case 9: _t->on_pushButton_clicked(); break;
        case 10: _t->on_stopButton_pressed(); break;
        case 11: _t->on_actionStop_triggered(); break;
        case 12: _t->on_fetchButton_pressed(); break;
        case 13: _t->on_setParameterButton_pressed(); break;
        case 14: _t->on_temp90Button_clicked(); break;
        case 15: _t->on_actionSetParameter_triggered(); break;
        default: ;
        }
    }
}

const QMetaObject Gui::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_Gui.data,
      qt_meta_data_Gui,  qt_static_metacall, 0, 0}
};


const QMetaObject *Gui::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Gui::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Gui.stringdata))
        return static_cast<void*>(const_cast< Gui*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int Gui::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
