#ifndef GAME_MENUES_H
#define GAME_MENUES_H

#include <QObject>

// forward declarations
class QWidget;
class QPushButton;
class QLineEdit;
class QCheckBox;
class QSpinBox;
class QStackedWidget;
class QListWidget;

class Game_Networking_Elements;
class Game_Multiplayer_Lobby_Elements;
class Shared_Elements;

class Game_Menues : public QObject
{
public:
    explicit Game_Menues(QWidget *parent = nullptr);

    // lets the menu system talk to shared state
    void Attach_Shared_Elements(Shared_Elements *sharedElements);

    // lets the menu system trigger networking actions
    void Attach_Networking_Elements(Game_Networking_Elements *networkingElements);

    // lets the menu system move into the actual lobby screen
    void Attach_Multiplayer_Lobby_Elements(Game_Multiplayer_Lobby_Elements *lobbyElements);

    // connects all the buttons to screen navigation / actions
    void Setup_Navigation();

    // root widget for the main window
    QWidget *Get_Root_Widget() const;

private:
    // screen movement
    void Go_To_Multiplayer_Menu();
    void Go_To_Main_Menu();
    void Go_To_Create_Lobby_Menu();
    void Go_To_Join_Server_Menu();
    void Go_To_Lobby_Menu();

    // actions
    void Start_Hosting();
    void Join_Game();
    void Push_Create_Lobby_Form_Data_Into_Shared_Elements();

    // stacked widget holds all screens
    QStackedWidget *Screen_Stack;

    // all screens
    QWidget *Main_Menu_Widget;
    QWidget *Multiplayer_Menu_Widget;
    QWidget *Create_Lobby_Menu_Widget;
    QWidget *Join_Server_Menu_Widget;

    // main menu -> multiplayer
    QPushButton *Main_Menu_Multiplayer_Button;

    // multiplayer screen buttons
    QPushButton *Multiplayer_Host_Button;
    QPushButton *Multiplayer_Client_Button;
    QPushButton *Multiplayer_Back_Button;

    // create lobby screen widgets
    QPushButton *Create_Lobby_Button;
    QPushButton *Create_Lobby_Back_Button;
    QLineEdit *Lobby_Name_Input;
    QSpinBox *Max_Players_Input;
    QCheckBox *Public_Lobby_Checkbox;

    // join server screen widgets
    QPushButton *Join_Server_Join_Button;
    QPushButton *Join_Server_Refresh_Button;
    QPushButton *Join_Server_Back_Button;
    QListWidget *Join_Server_Lobby_List;
    QLineEdit *Join_Server_Player_Name_Input;

    // attached systems
    Shared_Elements *Shared_Group;
    Game_Networking_Elements *Game_Networking_Group;
    Game_Multiplayer_Lobby_Elements *Game_Multiplayer_Lobby_Group;

    // just to stop connections being made twice
    bool Navigation_Is_Set_Up;
};

#endif // GAME_MENUES_H
