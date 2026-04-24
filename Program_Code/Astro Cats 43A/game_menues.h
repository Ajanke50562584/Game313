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

    void Attach_Shared_Elements(Shared_Elements *sharedElements);
    void Attach_Networking_Elements(Game_Networking_Elements *networkingElements);
    void Attach_Multiplayer_Lobby_Elements(Game_Multiplayer_Lobby_Elements *lobbyElements);
    void Setup_Navigation();
    QWidget *Get_Root_Widget() const;

private:
    void Build_Screens(QWidget *parent);
    void Cache_Screen_Controls();

    void Go_To_Multiplayer_Menu();
    void Go_To_Main_Menu();
    void Go_To_Create_Lobby_Menu();
    void Go_To_Join_Server_Menu();
    void Go_To_Lobby_Menu();

    void Start_Hosting();
    void Join_Game();
    void Push_Create_Lobby_Form_Data_Into_Shared_Elements();

    QStackedWidget *Screen_Stack;

    Menu_Main_Screen *Main_Menu_Screen;
    Menu_Multiplayer_Screen *Multiplayer_Menu_Screen;
    Menu_Create_Lobby_Screen *Create_Lobby_Menu_Screen;
    Menu_Join_Server_Screen *Join_Server_Menu_Screen;

    QWidget *Main_Menu_Widget;
    QWidget *Multiplayer_Menu_Widget;
    QWidget *Create_Lobby_Menu_Widget;
    QWidget *Join_Server_Menu_Widget;

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

    Shared_Elements *Shared_Group;
    Game_Networking_Elements *Game_Networking_Group;
    Game_Multiplayer_Lobby_Elements *Game_Multiplayer_Lobby_Group;

    bool Navigation_Is_Set_Up;
};

#endif // GAME_MENUES_H
