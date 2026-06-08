/****************************************************************************
** Meta object code from reading C++ file 'menu_create_lobby_screen.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.12)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Astro Cats 53A/menu_create_lobby_screen.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'menu_create_lobby_screen.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.12. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Menu_Create_Lobby_Screen_t {
    QByteArrayData data[9];
    char stringdata0[189];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Menu_Create_Lobby_Screen_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Menu_Create_Lobby_Screen_t qt_meta_stringdata_Menu_Create_Lobby_Screen = {
    {
QT_MOC_LITERAL(0, 0, 24), // "Menu_Create_Lobby_Screen"
QT_MOC_LITERAL(1, 25, 24), // "Handle_Host_Name_Changed"
QT_MOC_LITERAL(2, 50, 0), // ""
QT_MOC_LITERAL(3, 51, 4), // "text"
QT_MOC_LITERAL(4, 56, 27), // "Handle_Public_Lobby_Toggled"
QT_MOC_LITERAL(5, 84, 7), // "checked"
QT_MOC_LITERAL(6, 92, 28), // "Handle_Show_Password_Toggled"
QT_MOC_LITERAL(7, 121, 32), // "Set_Button_Cursor_To_Closed_Hand"
QT_MOC_LITERAL(8, 154, 34) // "Set_Button_Cursor_To_Pointing..."

    },
    "Menu_Create_Lobby_Screen\0"
    "Handle_Host_Name_Changed\0\0text\0"
    "Handle_Public_Lobby_Toggled\0checked\0"
    "Handle_Show_Password_Toggled\0"
    "Set_Button_Cursor_To_Closed_Hand\0"
    "Set_Button_Cursor_To_Pointing_Hand"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Menu_Create_Lobby_Screen[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x08 /* Private */,
       4,    1,   42,    2, 0x08 /* Private */,
       6,    1,   45,    2, 0x08 /* Private */,
       7,    0,   48,    2, 0x08 /* Private */,
       8,    0,   49,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Menu_Create_Lobby_Screen::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Menu_Create_Lobby_Screen *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Handle_Host_Name_Changed((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->Handle_Public_Lobby_Toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->Handle_Show_Password_Toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->Set_Button_Cursor_To_Closed_Hand(); break;
        case 4: _t->Set_Button_Cursor_To_Pointing_Hand(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Menu_Create_Lobby_Screen::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_Menu_Create_Lobby_Screen.data,
    qt_meta_data_Menu_Create_Lobby_Screen,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Menu_Create_Lobby_Screen::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Menu_Create_Lobby_Screen::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Menu_Create_Lobby_Screen.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Menu_Create_Lobby_Screen::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
