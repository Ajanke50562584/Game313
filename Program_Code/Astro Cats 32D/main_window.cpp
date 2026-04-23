#include "main_window.h"

#include "game_menues.h"
#include "game_multiplayer_lobby_elements.h"
#include "game_networking_elements.h"
#include "shared_elements.h"

#include <QDebug>
#include <QKeyEvent>

Main_Window::Main_Window(QWidget *parent)
    : QMainWindow(parent)
{
    Shared_Group = new Shared_Elements(this);
    Game_Menues_Group = new Game_Menues(this);
    Game_Multiplayer_Lobby_Group = new Game_Multiplayer_Lobby_Elements(this);
    Game_Networking_Group = new Game_Networking_Elements(this);

    Game_Multiplayer_Lobby_Group->Attach_Shared_Elements(Shared_Group);

    Game_Networking_Group->Attach_Shared_Elements(Shared_Group);
    Game_Networking_Group->Attach_Multiplayer_Lobby_Elements(Game_Multiplayer_Lobby_Group);

    Game_Menues_Group->Attach_Shared_Elements(Shared_Group);
    Game_Menues_Group->Attach_Networking_Elements(Game_Networking_Group);
    Game_Menues_Group->Attach_Multiplayer_Lobby_Elements(Game_Multiplayer_Lobby_Group);
    Game_Menues_Group->Setup_Navigation();

    setCentralWidget(Game_Menues_Group->Get_Root_Widget());

    resize(1280, 720);
    showNormal();
}

void Main_Window::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_F11:
        if(isFullScreen())
            showNormal();
        else
            showFullScreen();
        break;

    case Qt::Key_Escape:
        if(isFullScreen())
            showNormal();
        break;

    case Qt::Key_W:
        qDebug() << "climb";
        break;

    case Qt::Key_A:
        qDebug() << "Move Left";
        break;

    case Qt::Key_S:
        qDebug() << "croutch";
        break;

    case Qt::Key_D:
        qDebug() << "Move Right";
        break;
    }

    QMainWindow::keyPressEvent(event);
}

Main_Window::~Main_Window()
{
}
