/****************************************************************************
** Meta object code from reading C++ file 'dcpconnection.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "dcpconnection.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dcpconnection.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DCPConnection[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,
      42,   34,   14,   14, 0x05,
      83,   71,   14,   14, 0x05,
     127,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
     142,   14,   14,   14, 0x08,
     154,   14,   14,   14, 0x08,
     166,   71,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_DCPConnection[] = {
    "DCPConnection\0\0networkConnected()\0"
    "message\0messageReceived(DCPMessage*)\0"
    "socketError\0errorReceived(QAbstractSocket::SocketError)\0"
    "disconnected()\0connected()\0dataReady()\0"
    "handleError(QAbstractSocket::SocketError)\0"
};

void DCPConnection::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DCPConnection *_t = static_cast<DCPConnection *>(_o);
        switch (_id) {
        case 0: _t->networkConnected(); break;
        case 1: _t->messageReceived((*reinterpret_cast< DCPMessage*(*)>(_a[1]))); break;
        case 2: _t->errorReceived((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 3: _t->disconnected(); break;
        case 4: _t->connected(); break;
        case 5: _t->dataReady(); break;
        case 6: _t->handleError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DCPConnection::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DCPConnection::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DCPConnection,
      qt_meta_data_DCPConnection, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DCPConnection::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DCPConnection::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DCPConnection::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DCPConnection))
        return static_cast<void*>(const_cast< DCPConnection*>(this));
    return QObject::qt_metacast(_clname);
}

int DCPConnection::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void DCPConnection::networkConnected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void DCPConnection::messageReceived(DCPMessage * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DCPConnection::errorReceived(QAbstractSocket::SocketError _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DCPConnection::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
