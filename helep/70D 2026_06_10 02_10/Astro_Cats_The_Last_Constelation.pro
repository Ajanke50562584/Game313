QT       += core gui network opengl
LIBS     += -lopengl32  -lglu32

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

SOURCES += \
    game_menus.cpp \
    game_multiplayer_lobby_elements.cpp \
    game_networking_elements.cpp \
    leaderboard_data.cpp \
    leaderboard_screen.cpp \
    level.cpp \
    main.cpp \
    main_window.cpp \
    menu_create_lobby_screen.cpp \
    menu_join_server_screen.cpp \
    menu_main_screen.cpp \
    menu_multiplayer_screen.cpp \
    multiplayer_game_screen.cpp \
    platform.cpp \
    player.cpp \
    shared_elements.cpp \
    object_physics_body.cpp \
    story_mode_main_screen.cpp \
    single_player_game_screen.cpp

HEADERS += \
    game_menus.h \
    game_multiplayer_lobby_elements.h \
    game_networking_elements.h \
    leaderboard_data.h \
    leaderboard_screen.h \
    level.h \
    main_window.h \
    menu_create_lobby_screen.h \
    menu_join_server_screen.h \
    menu_main_screen.h \
    menu_multiplayer_screen.h \
    multiplayer_game_screen.h \
    platform.h \
    player.h \
    shared_elements.h \
    object_physics_body.h \
    story_mode_main_screen.h \
    single_player_game_screen.h

RESOURCES += \
    Resources.qrc
