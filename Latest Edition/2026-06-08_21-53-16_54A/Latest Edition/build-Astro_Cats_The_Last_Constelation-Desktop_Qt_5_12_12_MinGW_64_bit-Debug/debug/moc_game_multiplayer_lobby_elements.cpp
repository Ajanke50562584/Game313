/****************************************************************************
** Meta object code from reading C++ file 'game_multiplayer_lobby_elements.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.12)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Astro Cats 53A/game_multiplayer_lobby_elements.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'game_multiplayer_lobby_elements.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.12. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Game_Multiplayer_Lobby_Elements_t {
    QByteArrayData data[12];
    char stringdata0[246];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Game_Multiplayer_Lobby_Elements_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Game_Multiplayer_Lobby_Elements_t qt_meta_stringdata_Game_Multiplayer_Lobby_Elements = {
    {
QT_MOC_LITERAL(0, 0, 31), // "Game_Multiplayer_Lobby_Elements"
QT_MOC_LITERAL(1, 32, 21), // "Leave_Lobby_Requested"
QT_MOC_LITERAL(2, 54, 0), // ""
QT_MOC_LITERAL(3, 55, 30), // "Animate_Player_Status_Ellipses"
QT_MOC_LITERAL(4, 86, 12), // "Send_To_Chat"
QT_MOC_LITERAL(5, 99, 18), // "Handle_Leave_Lobby"
QT_MOC_LITERAL(6, 118, 26), // "Handle_Customize_Character"
QT_MOC_LITERAL(7, 145, 15), // "Handle_Vote_Map"
QT_MOC_LITERAL(8, 161, 20), // "Handle_Ready_Toggled"
QT_MOC_LITERAL(9, 182, 17), // "Handle_Start_Game"
QT_MOC_LITERAL(10, 200, 26), // "Handle_Edit_Lobby_Settings"
QT_MOC_LITERAL(11, 227, 18) // "Handle_Kick_Player"

    },
    "Game_Multiplayer_Lobby_Elements\0"
    "Leave_Lobby_Requested\0\0"
    "Animate_Player_Status_Ellipses\0"
    "Send_To_Chat\0Handle_Leave_Lobby\0"
    "Handle_Customize_Character\0Handle_Vote_Map\0"
    "Handle_Ready_Toggled\0Handle_Start_Game\0"
    "Handle_Edit_Lobby_Settings\0"
    "Handle_Kick_Player"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Game_Multiplayer_Lobby_Elements[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   65,    2, 0x08 /* Private */,
       4,    0,   66,    2, 0x08 /* Private */,
       5,    0,   67,    2, 0x08 /* Private */,
       6,    0,   68,    2, 0x08 /* Private */,
       7,    0,   69,    2, 0x08 /* Private */,
       8,    0,   70,    2, 0x08 /* Private */,
       9,    0,   71,    2, 0x08 /* Private */,
      10,    0,   72,    2, 0x08 /* Private */,
      11,    0,   73,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
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

void Game_Multiplayer_Lobby_Elements::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Game_Multiplayer_Lobby_Elements *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Leave_Lobby_Requested(); break;
        case 1: _t->Animate_Player_Status_Ellipses(); break;
        case 2: _t->Send_To_Chat(); break;
        case 3: _t->Handle_Leave_Lobby(); break;
        case 4: _t->Handle_Customize_Character(); break;
        case 5: _t->Handle_Vote_Map(); break;
        case 6: _t->Handle_Ready_Toggled(); break;
        case 7: _t->Handle_Start_Game(); break;
        case 8: _t->Handle_Edit_Lobby_Settings(); break;
        case 9: _t->Handle_Kick_Player(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Game_Multiplayer_Lobby_Elements::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Game_Multiplayer_Lobby_Elements::Leave_Lobby_Requested)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject Game_Multiplayer_Lobby_Elements::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_Game_Multiplayer_Lobby_Elements.data,
    qt_meta_data_Game_Multiplayer_Lobby_Elements,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Game_Multiplayer_Lobby_Elements::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Game_Multiplayer_Lobby_Elements::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Game_Multiplayer_Lobby_Elements.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Game_Multiplayer_Lobby_Elements::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void Game_Multiplayer_Lobby_Elements::Leave_Lobby_Requested()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
