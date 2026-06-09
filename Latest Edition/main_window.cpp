
#include "main_window.h"

#include "game_menus.h"
#include "game_multiplayer_lobby_elements.h"
#include "game_networking_elements.h"
#include "shared_elements.h"

#include <QApplication>
#include <QDebug>
#include <QEvent>
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
    qApp->installEventFilter(this);

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

bool Main_Window::eventFilter(QObject *object, QEvent *event)
{
    if(object != this && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *Key_Event = static_cast<QKeyEvent *>(event);

        switch(Key_Event->key())
        {
        case Qt::Key_F11:
        case Qt::Key_Escape:
            keyPressEvent(Key_Event);
            return true;

        case Qt::Key_W:
        case Qt::Key_Up:
        case Qt::Key_A:
        case Qt::Key_Left:
        case Qt::Key_S:
        case Qt::Key_Down:
        case Qt::Key_D:
        case Qt::Key_Right:
        case Qt::Key_Space:
            if(Game_Menus_Group->Is_Story_Mode_Tutorial_Current())
            {
                keyPressEvent(Key_Event);
                return true;
            }
            break;
        }
    }

    if(object != this && event->type() == QEvent::KeyRelease)
    {
        QKeyEvent *Key_Event = static_cast<QKeyEvent *>(event);

        switch(Key_Event->key())
        {
        case Qt::Key_W:
        case Qt::Key_Up:
        case Qt::Key_A:
        case Qt::Key_Left:
        case Qt::Key_S:
        case Qt::Key_Down:
        case Qt::Key_D:
        case Qt::Key_Right:
        case Qt::Key_Space:
            if(Game_Menus_Group->Is_Story_Mode_Tutorial_Current())
            {
                keyReleaseEvent(Key_Event);
                return true;
            }
            break;
        }
    }

    return QMainWindow::eventFilter(object, event);
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

    case Qt::Key_W:
    case Qt::Key_Up:
        qDebug() << "climb";
        if(Game_Menus_Group->Is_Story_Mode_Tutorial_Current())
            Game_Menus_Group->Get_Story_Mode_Tutorial_Screen()->Set_Move_Climb_Pressed(true);
        break;

    case Qt::Key_A:
    case Qt::Key_Left:
        qDebug() << "Move Left";
        if(Game_Menus_Group->Is_Story_Mode_Tutorial_Current())
            Game_Menus_Group->Get_Story_Mode_Tutorial_Screen()->Set_Move_Left_Pressed(true);
        break;

    case Qt::Key_S:
    case Qt::Key_Down:
        qDebug() << "croutch";
        if(Game_Menus_Group->Is_Story_Mode_Tutorial_Current())
            Game_Menus_Group->Get_Story_Mode_Tutorial_Screen()->Set_Move_Crouch_Pressed(true);
        break;

    case Qt::Key_D:
    case Qt::Key_Right:
        qDebug() << "Move Right";
        if(Game_Menus_Group->Is_Story_Mode_Tutorial_Current())
            Game_Menus_Group->Get_Story_Mode_Tutorial_Screen()->Set_Move_Right_Pressed(true);
        break;

    case Qt::Key_Space:
        qDebug() << "Jump";
        if(Game_Menus_Group->Is_Story_Mode_Tutorial_Current())
            Game_Menus_Group->Get_Story_Mode_Tutorial_Screen()->Set_Move_Jump_Pressed(true);
        break;

    }

    QMainWindow::keyPressEvent(event);
}

void Main_Window::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_W:
    case Qt::Key_Up:
        qDebug() << "climb Released";
        if(Game_Menus_Group->Is_Story_Mode_Tutorial_Current())
            Game_Menus_Group->Get_Story_Mode_Tutorial_Screen()->Set_Move_Climb_Pressed(false);
        break;

    case Qt::Key_A:
    case Qt::Key_Left:
        qDebug() << "Move Left Released";
        if(Game_Menus_Group->Is_Story_Mode_Tutorial_Current())
            Game_Menus_Group->Get_Story_Mode_Tutorial_Screen()->Set_Move_Left_Pressed(false);
        break;

    case Qt::Key_S:
    case Qt::Key_Down:
        qDebug() << "croutch Released";
        if(Game_Menus_Group->Is_Story_Mode_Tutorial_Current())
            Game_Menus_Group->Get_Story_Mode_Tutorial_Screen()->Set_Move_Crouch_Pressed(false);
        break;

    case Qt::Key_D:
    case Qt::Key_Right:
        qDebug() << "Move Right Released";
        if(Game_Menus_Group->Is_Story_Mode_Tutorial_Current())
            Game_Menus_Group->Get_Story_Mode_Tutorial_Screen()->Set_Move_Right_Pressed(false);
        break;

    case Qt::Key_Space:
        qDebug() << "Jump Released";
        if(Game_Menus_Group->Is_Story_Mode_Tutorial_Current())
            Game_Menus_Group->Get_Story_Mode_Tutorial_Screen()->Set_Move_Jump_Pressed(false);
        break;

    }

    QMainWindow::keyReleaseEvent(event);
}

Main_Window::~Main_Window()
{
    // qt parent ownership handles cleanup
}
