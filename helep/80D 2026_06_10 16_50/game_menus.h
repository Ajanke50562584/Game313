#ifndef GAME_MENUES_H
#define GAME_MENUES_H

#include <QObject> // signals and slots
#include <QCheckBox> // For private loby box
#include <QWidget> // screens are widgets
#include <QPushButton> // buttons from all menus
#include <QLineEdit> // form inputs
#include <QSpinBox> // max players input
#include <QStackedWidget> // for the screen stack
#include <QListWidget> // For the join lobby list
#include <QList>

#include "game_networking_elements.h" // menu starts/stops networking flows
#include "game_multiplayer_lobby_elements.h" // lobby screen controller
#include "shared_elements.h" // shared lobby data

// All of these are screens that get put on screenstack
#include "menu_main_screen.h" // main menu
#include "menu_multiplayer_screen.h" // host/join choice
#include "menu_create_lobby_screen.h" // create lobby form
#include "menu_join_server_screen.h" // join lobby browser
#include "Story_Mode_Main_Screen.h" // single player level select
#include "single_player_game_screen.h" // local game screen
#include "multiplayer_game_screen.h" // multiplayer game screen
#include "leaderboard_screen.h" // highscore screen

class Game_Menus : public QObject // controls all screen switching
{
    Q_OBJECT // Signals and slots

public:
    explicit Game_Menus(QWidget *parent = nullptr); // builds screen stack and screens

    // All multiplauer networking stuff
    void Attach_Shared_Elements(Shared_Elements *sharedElements); // gives menus shared data
    void Attach_Networking_Elements(Game_Networking_Elements *networkingElements); // gives menus networking
    void Attach_Multiplayer_Lobby_Elements(Game_Multiplayer_Lobby_Elements *lobbyElements); // gives menus lobby screen

    // connect all menu navigation
    void Setup_Navigation();

    // To get the stackedwidget's pointer
    QWidget *Get_Root_Widget() const; // main window uses this as central widget
    bool Is_Single_Player_Game_Current() const; // true if single player screen showing
    bool Is_Multiplayer_Game_Current() const; // true if multiplayer game screen showing
    Single_Player_Game_Screen *Get_Single_Player_Game_Screen() const; // used for input forwarding
    Multiplayer_Game_Screen *Get_Multiplayer_Game_Screen() const; // used for input forwarding

private:
    // cache the buttons and inputs
    void Cache_Screen_Controls();

    // screen swap helpers
    void Go_To_Main_Menu(); // show main menu
    void Go_To_Create_Lobby_Menu(); // show host form
    void Go_To_Join_Server_Menu(); // show join browser
    void Go_To_Lobby_Menu(); // show lobby widget
    void Go_To_Story_Mode(); // show story menu
    void Go_To_Leaderboard(); // show highscores



    // host / join entry points
    void Start_Hosting(); // copy form data and bind host socket
    void Join_Game(); // join selected discovered lobby

    // copy create lobby form into shared state
    void Push_Create_Lobby_Form_Data_Into_Shared_Elements(); // form -> shared state

private slots:
    // button click slots
    void Go_To_Multiplayer_Menu(); // show multiplayer menu
    void Main_Menu_Story_Mode_Button_Clicked(); // story button handler
    void Main_Menu_Multiplayer_Button_Clicked(); // multiplayer button handler
    void Multiplayer_Back_Button_Clicked(); // back from multiplayer
    void Multiplayer_Host_Button_Clicked(); // host chosen
    void Multiplayer_Client_Button_Clicked(); // join chosen
    void Create_Lobby_Back_Button_Clicked(); // back from create lobby
    void Create_Lobby_Button_Clicked(); // create lobby pressed
    void Join_Server_Join_Button_Clicked(); // join pressed
    void Join_Server_Refresh_Button_Clicked(); // refresh lobbies pressed
    void Join_Server_Back_Button_Clicked(); // back from join screen
    void Tutorial_Button_Clicked(); // tutorial level pressed
    void Select_Level_Button_Clicked(); // normal level pressed
    void Story_Level_Button_Clicked(); // specific story level pressed
    void Level_Select_Back_Clicked(); // back from story menu
    void Highscores_Button_Clicked(); // highscores pressed
    void Leaderboard_Back_Clicked(); // back from leaderboard
    void Go_To_Single_Player_Game(); // switch to single player game
    void Go_To_Multiplayer_Game(); // switch to multiplayer game


    // refresh the lobby list after replies come in
    void Refresh_Discovered_Lobbies();

private:
    // stacked widget for menu flow
    QStackedWidget *Screen_Stack; // holds all menus/screens

    // screen objects
    Menu_Main_Screen *Main_Menu_Screen; // main menu object
    Menu_Multiplayer_Screen *Multiplayer_Menu_Screen; // host/join screen
    Menu_Create_Lobby_Screen *Create_Lobby_Menu_Screen; // create lobby screen
    Menu_Join_Server_Screen *Join_Server_Menu_Screen; // join server screen
    Story_Mode_Main_Screen *Story_Mode_Main_Menu_Screen; // story menu screen
    Single_Player_Game_Screen *Single_Player_Game_Screen_Widget; // single player game
    Multiplayer_Game_Screen *Multiplayer_Game_Screen_Widget; // multiplayer game
    Leaderboard_Screen *Leaderboard_Menu_Screen; // leaderboard screen

    // screen widgets for switching
    QWidget *Main_Menu_Widget; // main menu as widget
    QWidget *Multiplayer_Menu_Widget; // multiplayer menu as widget
    QWidget *Create_Lobby_Menu_Widget; // host screen as widget
    QWidget *Join_Server_Menu_Widget; // join screen as widget

    // cached controls for navigation and form reads
    QPushButton *Main_Menu_Story_Mode_Button; // cached story button
    QPushButton *Main_Menu_Multiplayer_Button; // cached multiplayer button
    QPushButton *Main_Menu_Highscores_Button; // cached highscores button
    QPushButton *Multiplayer_Host_Button; // cached host button
    QPushButton *Multiplayer_Client_Button; // cached client button
    QPushButton *Multiplayer_Back_Button; // cached multiplayer back button
    QPushButton *Create_Lobby_Button; // cached create lobby button
    QPushButton *Create_Lobby_Back_Button; // cached create back button
    QLineEdit *Lobby_Name_Input; // cached lobby name textbox
    QLineEdit *Host_Name_Input; // cached host name textbox
    QSpinBox *Max_Players_Input; // cached max player spinner
    QCheckBox *Public_Lobby_Checkbox; // cached public toggle
    QPushButton *Join_Server_Join_Button; // cached join button
    QPushButton *Join_Server_Refresh_Button; // cached refresh button
    QPushButton *Join_Server_Back_Button; // cached join back button
    QListWidget *Join_Server_Lobby_List; // cached lobby list
    QLineEdit *Join_Server_Player_Name_Input; // cached player name input
    QPushButton *Tutorial_Button; // cached tutorial button
    QPushButton *Select_Level_Button; // cached level button
    QList<QPushButton*> Story_Level_Buttons;
    QPushButton *Level_Select_Back_Button; // cached story back button
    QPushButton *Leaderboard_Back_Button; // cached leaderboard back button

    // shared systems used by menus
    Shared_Elements *Shared_Group; // shared lobby state
    Game_Networking_Elements *Game_Networking_Group; // networking system
    Game_Multiplayer_Lobby_Elements *Game_Multiplayer_Lobby_Group; // lobby ui system

    // stops duplicate signal wiring
    bool Navigation_Is_Set_Up; // true after connect calls happen
};

#endif // GAME_MENUES_H
