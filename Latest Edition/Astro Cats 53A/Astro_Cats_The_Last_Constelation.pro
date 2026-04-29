QT       += core gui network opengl
LIBS     += -lopengl32  -lglu32

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

SOURCES += \
    game_menus.cpp \
    game_multiplayer_lobby_elements.cpp \
    game_networking_elements.cpp \
    main.cpp \
    main_window.cpp \
    menu_create_lobby_screen.cpp \
    menu_join_server_screen.cpp \
    menu_main_screen.cpp \
    menu_multiplayer_screen.cpp \
    player.cpp \
    shared_elements.cpp \
    object_physics_body.cpp \
    single_player_tutorial_scene.cpp \
    story_mode_main_screen.cpp \
    story_mode_tutorial.cpp

HEADERS += \
    game_menus.h \
    game_multiplayer_lobby_elements.h \
    game_networking_elements.h \
    main_window.h \
    menu_create_lobby_screen.h \
    menu_join_server_screen.h \
    menu_main_screen.h \
    menu_multiplayer_screen.h \
    player.h \
    shared_elements.h \
    object_physics_body.h \
    single_player_tutorial_scene.h \
    story_mode_main_screen.h \
    story_mode_tutorial.h

RESOURCES += \
    Resources.qrc
