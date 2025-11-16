/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.15)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.15. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[24];
    char stringdata0[364];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 27), // "on_pushButton_Start_clicked"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 26), // "on_pushButton_Stop_clicked"
QT_MOC_LITERAL(4, 67, 27), // "on_pushButton_Pause_clicked"
QT_MOC_LITERAL(5, 95, 27), // "on_pushButton_Reset_clicked"
QT_MOC_LITERAL(6, 123, 21), // "onStationStatusUpdate"
QT_MOC_LITERAL(7, 145, 9), // "stationId"
QT_MOC_LITERAL(8, 155, 6), // "status"
QT_MOC_LITERAL(9, 162, 27), // "onProductFinishedProcessing"
QT_MOC_LITERAL(10, 190, 7), // "Product"
QT_MOC_LITERAL(11, 198, 7), // "product"
QT_MOC_LITERAL(12, 206, 11), // "stationName"
QT_MOC_LITERAL(13, 218, 22), // "onProductionLineStatus"
QT_MOC_LITERAL(14, 241, 3), // "msg"
QT_MOC_LITERAL(15, 245, 13), // "onNewLogEntry"
QT_MOC_LITERAL(16, 259, 7), // "message"
QT_MOC_LITERAL(17, 267, 13), // "updateMetrics"
QT_MOC_LITERAL(18, 281, 19), // "handleThreadMessage"
QT_MOC_LITERAL(19, 301, 4), // "name"
QT_MOC_LITERAL(20, 306, 6), // "action"
QT_MOC_LITERAL(21, 313, 24), // "handleSystemResetRequest"
QT_MOC_LITERAL(22, 338, 15), // "handleStatsData"
QT_MOC_LITERAL(23, 354, 9) // "statsJson"

    },
    "MainWindow\0on_pushButton_Start_clicked\0"
    "\0on_pushButton_Stop_clicked\0"
    "on_pushButton_Pause_clicked\0"
    "on_pushButton_Reset_clicked\0"
    "onStationStatusUpdate\0stationId\0status\0"
    "onProductFinishedProcessing\0Product\0"
    "product\0stationName\0onProductionLineStatus\0"
    "msg\0onNewLogEntry\0message\0updateMetrics\0"
    "handleThreadMessage\0name\0action\0"
    "handleSystemResetRequest\0handleStatsData\0"
    "statsJson"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x08 /* Private */,
       3,    0,   75,    2, 0x08 /* Private */,
       4,    0,   76,    2, 0x08 /* Private */,
       5,    0,   77,    2, 0x08 /* Private */,
       6,    2,   78,    2, 0x08 /* Private */,
       9,    2,   83,    2, 0x08 /* Private */,
      13,    1,   88,    2, 0x08 /* Private */,
      15,    1,   91,    2, 0x08 /* Private */,
      17,    0,   94,    2, 0x08 /* Private */,
      18,    3,   95,    2, 0x08 /* Private */,
      21,    0,  102,    2, 0x08 /* Private */,
      22,    1,  103,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    7,    8,
    QMetaType::Void, 0x80000000 | 10, QMetaType::QString,   11,   12,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   16,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,   19,   20,   16,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   23,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_pushButton_Start_clicked(); break;
        case 1: _t->on_pushButton_Stop_clicked(); break;
        case 2: _t->on_pushButton_Pause_clicked(); break;
        case 3: _t->on_pushButton_Reset_clicked(); break;
        case 4: _t->onStationStatusUpdate((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 5: _t->onProductFinishedProcessing((*reinterpret_cast< const Product(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 6: _t->onProductionLineStatus((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->onNewLogEntry((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->updateMetrics(); break;
        case 9: _t->handleThreadMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 10: _t->handleSystemResetRequest(); break;
        case 11: _t->handleStatsData((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
