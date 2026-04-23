QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

SOURCES += \
    game_menues.cpp \
    game_multiplayer_lobby_elements.cpp \
    game_networking_elements.cpp \
    main.cpp \
    main_window.cpp \
    menu_create_lobby_screen.cpp \
    menu_join_server_screen.cpp \
    menu_main_screen.cpp \
    menu_multiplayer_screen.cpp \
    shared_elements.cpp

HEADERS += \
    game_menues.h \
    game_multiplayer_lobby_elements.h \
    game_networking_elements.h \
    main_window.h \
    menu_create_lobby_screen.h \
    menu_join_server_screen.h \
    menu_main_screen.h \
    menu_multiplayer_screen.h \
    shared_elements.h

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
