
#include "main_window.h"

#include "game_menus.h"
#include "game_multiplayer_lobby_elements.h"
#include "game_networking_elements.h"
#include "shared_elements.h"
#include "single_player_tutorial_scene.h"

#include <QDebug>
#include <QGuiApplication>
#include <QKeyEvent>
#include <QScreen>

Main_Window::Main_Window(QWidget *parent)
    : QMainWindow(parent)
{
    // create the main shared systems
    Shared_Group = new Shared_Elements(this);
    Game_Menus_Group = new Game_Menus(this);
    Game_Multiplayer_Lobby_Group = new Game_Multiplayer_Lobby_Elements(this);
    Game_Networking_Group = new Game_Networking_Elements(this);

    // attach shared data
    Game_Multiplayer_Lobby_Group->Attach_Shared_Elements(Shared_Group);
    Game_Networking_Group->Attach_Shared_Elements(Shared_Group);

    // connect lobby and networking to each other
    Game_Networking_Group->Attach_Multiplayer_Lobby_Elements(Game_Multiplayer_Lobby_Group);
    Game_Multiplayer_Lobby_Group->Attach_Networking_Elements(Game_Networking_Group);

    // then wire the menu layer into everything else
    Game_Menus_Group->Attach_Shared_Elements(Shared_Group);
    Game_Menus_Group->Attach_Networking_Elements(Game_Networking_Group);
    Game_Menus_Group->Attach_Multiplayer_Lobby_Elements(Game_Multiplayer_Lobby_Group);
    Game_Menus_Group->Setup_Navigation();

    // set the stacked menu system as the main content
    setCentralWidget(Game_Menus_Group->Get_Root_Widget());

    // start from the current screen size while keeping the same ratio
    const QRect Available_Geometry = QGuiApplication::primaryScreen()->availableGeometry();
    int Window_Width = (Available_Geometry.width() * 4) / 5;
    int Window_Height = (Window_Width * 9) / 16;

    if(Window_Height > (Available_Geometry.height() * 4) / 5)
    {
        Window_Height = (Available_Geometry.height() * 4) / 5;
        Window_Width = (Window_Height * 16) / 9;
    }

    resize(Window_Width, Window_Height);
    showNormal();
}

void Main_Window::keyPressEvent(QKeyEvent *event)
{
    // debug and window shortcuts
    switch(event->key())
    {
    case Qt::Key_F11:
        // toggle fullscreen
        if(isFullScreen())
            showNormal();
        else
            showFullScreen();
        break;

    case Qt::Key_Escape:
        // escape leaves fullscreen
        if(isFullScreen())
            showNormal();
        break;

    }

    QMainWindow::keyPressEvent(event);
}

Main_Window::~Main_Window()
{
    // qt parent ownership handles cleanup
}
