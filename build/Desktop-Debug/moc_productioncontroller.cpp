/****************************************************************************
** Meta object code from reading C++ file 'productioncontroller.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.15)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../productioncontroller.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'productioncontroller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.15. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ProductionController_t {
    QByteArrayData data[15];
    char stringdata0[216];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ProductionController_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ProductionController_t qt_meta_stringdata_ProductionController = {
    {
QT_MOC_LITERAL(0, 0, 20), // "ProductionController"
QT_MOC_LITERAL(1, 21, 17), // "newProductCreated"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 7), // "Product"
QT_MOC_LITERAL(4, 48, 7), // "product"
QT_MOC_LITERAL(5, 56, 20), // "productionLineStatus"
QT_MOC_LITERAL(6, 77, 6), // "status"
QT_MOC_LITERAL(7, 84, 19), // "updateStationStatus"
QT_MOC_LITERAL(8, 104, 9), // "stationId"
QT_MOC_LITERAL(9, 114, 19), // "productStateChanged"
QT_MOC_LITERAL(10, 134, 11), // "stationName"
QT_MOC_LITERAL(11, 146, 15), // "startProduction"
QT_MOC_LITERAL(12, 162, 14), // "stopProduction"
QT_MOC_LITERAL(13, 177, 15), // "generateProduct"
QT_MOC_LITERAL(14, 193, 22) // "onFinalProductFinished"

    },
    "ProductionController\0newProductCreated\0"
    "\0Product\0product\0productionLineStatus\0"
    "status\0updateStationStatus\0stationId\0"
    "productStateChanged\0stationName\0"
    "startProduction\0stopProduction\0"
    "generateProduct\0onFinalProductFinished"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ProductionController[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,
       5,    1,   57,    2, 0x06 /* Public */,
       7,    2,   60,    2, 0x06 /* Public */,
       9,    2,   65,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    0,   70,    2, 0x0a /* Public */,
      12,    0,   71,    2, 0x0a /* Public */,
      13,    0,   72,    2, 0x0a /* Public */,
      14,    2,   73,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    8,    6,
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString,    4,   10,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString,    4,   10,

       0        // eod
};

void ProductionController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ProductionController *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->newProductCreated((*reinterpret_cast< const Product(*)>(_a[1]))); break;
        case 1: _t->productionLineStatus((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->updateStationStatus((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 3: _t->productStateChanged((*reinterpret_cast< const Product(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 4: _t->startProduction(); break;
        case 5: _t->stopProduction(); break;
        case 6: _t->generateProduct(); break;
        case 7: _t->onFinalProductFinished((*reinterpret_cast< const Product(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ProductionController::*)(const Product & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ProductionController::newProductCreated)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ProductionController::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ProductionController::productionLineStatus)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ProductionController::*)(int , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ProductionController::updateStationStatus)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (ProductionController::*)(const Product & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ProductionController::productStateChanged)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ProductionController::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ProductionController.data,
    qt_meta_data_ProductionController,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ProductionController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ProductionController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ProductionController.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ProductionController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

// SIGNAL 0
void ProductionController::newProductCreated(const Product & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ProductionController::productionLineStatus(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ProductionController::updateStationStatus(int _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ProductionController::productStateChanged(const Product & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
