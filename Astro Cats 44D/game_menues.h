#ifndef GAME_MENUES_H
#define GAME_MENUES_H

#include <QObject>
#include <QCheckBox>

class QWidget;
class QPushButton;
class QLineEdit;
class QSpinBox;
class QStackedWidget;
class QListWidget;

class Game_Networking_Elements;
class Game_Multiplayer_Lobby_Elements;
class Shared_Elements;
class Menu_Main_Screen;
class Menu_Multiplayer_Screen;
class Menu_Create_Lobby_Screen;
class Menu_Join_Server_Screen;

class Game_Menues : public QObject
{
public:
    explicit Game_Menues(QWidget *parent = nullptr);

    // shared state for menu flow
    void Attach_Shared_Elements(Shared_Elements *sharedElements);

    // networking ref for host / join actions
    void Attach_Networking_Elements(Game_Networking_Elements *networkingElements);

    // lobby screen ref for stacked widget switching
    void Attach_Multiplayer_Lobby_Elements(Game_Multiplayer_Lobby_Elements *lobbyElements);

    // connect all menu navigation
    void Setup_Navigation();

    // root widget for main window
    QWidget *Get_Root_Widget() const;

private:
    // build all menu screens and add them to the stack
    void Build_Screens(QWidget *parent);

    // cache the buttons and inputs we use later
    void Cache_Screen_Controls();

    // screen swap helpers
    void Go_To_Multiplayer_Menu();
    void Go_To_Main_Menu();
    void Go_To_Create_Lobby_Menu();
    void Go_To_Join_Server_Menu();
    void Go_To_Lobby_Menu();

    // host / join entry points
    void Start_Hosting();
    void Join_Game();

    // copy create lobby form into shared state
    void Push_Create_Lobby_Form_Data_Into_Shared_Elements();

    // stacked widget for menu flow
    QStackedWidget *Screen_Stack;

    // screen objects
    Menu_Main_Screen *Main_Menu_Screen;
    Menu_Multiplayer_Screen *Multiplayer_Menu_Screen;
    Menu_Create_Lobby_Screen *Create_Lobby_Menu_Screen;
    Menu_Join_Server_Screen *Join_Server_Menu_Screen;

    // screen widgets for switching
    QWidget *Main_Menu_Widget;
    QWidget *Multiplayer_Menu_Widget;
    QWidget *Create_Lobby_Menu_Widget;
    QWidget *Join_Server_Menu_Widget;

    // cached controls for navigation and form reads
    QPushButton *Main_Menu_Multiplayer_Button;
    QPushButton *Multiplayer_Host_Button;
    QPushButton *Multiplayer_Client_Button;
    QPushButton *Multiplayer_Back_Button;
    QPushButton *Create_Lobby_Button;
    QPushButton *Create_Lobby_Back_Button;
    QLineEdit *Lobby_Name_Input;
    QSpinBox *Max_Players_Input;
    QCheckBox *Public_Lobby_Checkbox;
    QPushButton *Join_Server_Join_Button;
    QPushButton *Join_Server_Refresh_Button;
    QPushButton *Join_Server_Back_Button;
    QListWidget *Join_Server_Lobby_List;
    QLineEdit *Join_Server_Player_Name_Input;

    // shared systems used by menus
    Shared_Elements *Shared_Group;
    Game_Networking_Elements *Game_Networking_Group;
    Game_Multiplayer_Lobby_Elements *Game_Multiplayer_Lobby_Group;

    // stops duplicate signal wiring
    bool Navigation_Is_Set_Up;
};

#endif // GAME_MENUES_H
