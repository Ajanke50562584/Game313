#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

class QKeyEvent;
class Shared_Elements;
class Game_Menues;
class Game_Networking_Elements;
class Game_Multiplayer_Lobby_Elements;

class Main_Window : public QMainWindow
{
public:
    Main_Window(QWidget *parent = nullptr);
    ~Main_Window();

protected:
    // keyboard shortcuts for fullscreen and movement debug prints
    void keyPressEvent(QKeyEvent *event) override;

private:
    // main shared systems
    Shared_Elements *Shared_Group;
    Game_Menues *Game_Menues_Group;
    Game_Networking_Elements *Game_Networking_Group;
    Game_Multiplayer_Lobby_Elements *Game_Multiplayer_Lobby_Group;
};

#endif // MAIN_WINDOW_H
