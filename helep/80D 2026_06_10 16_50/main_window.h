#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow> // main app window
#include <QEvent> // global input filter
#include <QKeyEvent> // keyboard controll
#include <QMouseEvent> // mouse attacks

#include "shared_elements.h" // shared game and lobby state
#include "game_menus.h" // screen manager
#include "game_networking_elements.h" // udp networking
#include "game_multiplayer_lobby_elements.h" // lobby controller

class Main_Window : public QMainWindow // top level window that owns the main systems
{
public:
    Main_Window(QWidget *parent = nullptr); // builds all major game systems
    ~Main_Window(); // qt parent cleanup handles widgets

protected:
    bool eventFilter(QObject *object, QEvent *event) override; // catches input even when child widget has focus
    void keyPressEvent(QKeyEvent *event) override; // sends movement key pressed to current game
    void keyReleaseEvent(QKeyEvent *event) override; // sends movement key released to current game
    void mousePressEvent(QMouseEvent *event) override; // sends mouse attack pressed
    void mouseReleaseEvent(QMouseEvent *event) override; // sends mouse attack released

private:
    Shared_Elements *Shared_Group; // stores lobby/game info everyone needs
    Game_Menus *Game_Menus_Group; // owns screen stack and navigation
    Game_Networking_Elements *Game_Networking_Group; // sends and receives packets
    Game_Multiplayer_Lobby_Elements *Game_Multiplayer_Lobby_Group; // handles visible lobby widgets
};

#endif // MAIN_WINDOW_H
