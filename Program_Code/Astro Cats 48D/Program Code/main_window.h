#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QKeyEvent>

#include "shared_elements.h"
#include "game_menues.h"
#include "game_networking_elements.h"
#include "game_multiplayer_lobby_elements.h"

class Main_Window : public QMainWindow
{
public:
    Main_Window(QWidget *parent = nullptr);
    ~Main_Window();

protected:
    // keyboard controll setup
    void keyPressEvent(QKeyEvent *event) override;

private:
    // main shared systems
    Shared_Elements *Shared_Group;
    Game_Menues *Game_Menues_Group;
    Game_Networking_Elements *Game_Networking_Group;
    Game_Multiplayer_Lobby_Elements *Game_Multiplayer_Lobby_Group;
};

#endif // MAIN_WINDOW_H
