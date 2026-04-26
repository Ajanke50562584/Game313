QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

SOURCES += \
    "Program Code/game_menues.cpp" \
    "Program Code/game_multiplayer_lobby_elements.cpp" \
    "Program Code/game_networking_elements.cpp" \
    "Program Code/main.cpp" \
    "Program Code/main_window.cpp" \
    "Program Code/menu_create_lobby_screen.cpp" \
    "Program Code/menu_join_server_screen.cpp" \
    "Program Code/menu_main_screen.cpp" \
    "Program Code/menu_multiplayer_screen.cpp" \
    "Program Code/shared_elements.cpp"

HEADERS += \
    "Program Code/game_menues.h" \
    "Program Code/game_multiplayer_lobby_elements.h" \
    "Program Code/game_networking_elements.h" \
    "Program Code/main_window.h" \
    "Program Code/menu_create_lobby_screen.h" \
    "Program Code/menu_join_server_screen.h" \
    "Program Code/menu_main_screen.h" \
    "Program Code/menu_multiplayer_screen.h" \
    "Program Code/shared_elements.h"

INCLUDEPATH += "Program Code"
DEPENDPATH += "Program Code"
DESTDIR = $$PWD

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    "Resources/Resources.qrc"
