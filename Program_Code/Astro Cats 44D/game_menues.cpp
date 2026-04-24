#include "game_menues.h"

#include "game_multiplayer_lobby_elements.h"
#include "game_networking_elements.h"
#include "menu_create_lobby_screen.h"
#include "menu_join_server_screen.h"
#include "menu_main_screen.h"
#include "menu_multiplayer_screen.h"
#include "shared_elements.h"

#include <QDebug>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QSpinBox>
#include <QStackedWidget>
#include <QTimer>
#include <QWidget>

Game_Menues::Game_Menues(QWidget *parent)
    : QObject(parent)
    , Screen_Stack(nullptr)
    , Main_Menu_Screen(nullptr)
    , Multiplayer_Menu_Screen(nullptr)
    , Create_Lobby_Menu_Screen(nullptr)
    , Join_Server_Menu_Screen(nullptr)
    , Main_Menu_Widget(nullptr)
    , Multiplayer_Menu_Widget(nullptr)
    , Create_Lobby_Menu_Widget(nullptr)
    , Join_Server_Menu_Widget(nullptr)
    , Main_Menu_Multiplayer_Button(nullptr)
    , Multiplayer_Host_Button(nullptr)
    , Multiplayer_Client_Button(nullptr)
    , Multiplayer_Back_Button(nullptr)
    , Create_Lobby_Button(nullptr)
    , Create_Lobby_Back_Button(nullptr)
    , Lobby_Name_Input(nullptr)
    , Max_Players_Input(nullptr)
    , Public_Lobby_Checkbox(nullptr)
    , Join_Server_Join_Button(nullptr)
    , Join_Server_Refresh_Button(nullptr)
    , Join_Server_Back_Button(nullptr)
    , Join_Server_Lobby_List(nullptr)
    , Join_Server_Player_Name_Input(nullptr)
    , Shared_Group(nullptr)
    , Game_Networking_Group(nullptr)
    , Game_Multiplayer_Lobby_Group(nullptr)
    , Navigation_Is_Set_Up(false)
{
    // build the stacked widget flow
    Build_Screens(parent);

    // cache controls we need later
    Cache_Screen_Controls();
}

void Game_Menues::Build_Screens(QWidget *parent)
{
    // one stack handles screen switching
    Screen_Stack = new QStackedWidget(parent);

    // create each menu screen
    Main_Menu_Screen = new Menu_Main_Screen(Screen_Stack);
    Multiplayer_Menu_Screen = new Menu_Multiplayer_Screen(Screen_Stack);
    Create_Lobby_Menu_Screen = new Menu_Create_Lobby_Screen(Screen_Stack);
    Join_Server_Menu_Screen = new Menu_Join_Server_Screen(Screen_Stack);

    // widget refs for screen switching
    Main_Menu_Widget = Main_Menu_Screen;
    Multiplayer_Menu_Widget = Multiplayer_Menu_Screen;
    Create_Lobby_Menu_Widget = Create_Lobby_Menu_Screen;
    Join_Server_Menu_Widget = Join_Server_Menu_Screen;

    // add screens and start on main menu
    Screen_Stack->addWidget(Main_Menu_Widget);
    Screen_Stack->addWidget(Multiplayer_Menu_Widget);
    Screen_Stack->addWidget(Create_Lobby_Menu_Widget);
    Screen_Stack->addWidget(Join_Server_Menu_Widget);
    Screen_Stack->setCurrentWidget(Main_Menu_Widget);
}

void Game_Menues::Cache_Screen_Controls()
{
    // cache buttons and inputs from each screen
    Main_Menu_Multiplayer_Button = Main_Menu_Screen->Get_Multiplayer_Button();
    Multiplayer_Host_Button = Multiplayer_Menu_Screen->Get_Host_Button();
    Multiplayer_Client_Button = Multiplayer_Menu_Screen->Get_Client_Button();
    Multiplayer_Back_Button = Multiplayer_Menu_Screen->Get_Back_Button();

    Create_Lobby_Button = Create_Lobby_Menu_Screen->Get_Create_Lobby_Button();
    Create_Lobby_Back_Button = Create_Lobby_Menu_Screen->Get_Back_Button();
    Lobby_Name_Input = Create_Lobby_Menu_Screen->Get_Lobby_Name_Input();
    Max_Players_Input = Create_Lobby_Menu_Screen->Get_Max_Players_Input();
    Public_Lobby_Checkbox = Create_Lobby_Menu_Screen->Get_Public_Lobby_Checkbox();

    Join_Server_Join_Button = Join_Server_Menu_Screen->Get_Join_Button();
    Join_Server_Refresh_Button = Join_Server_Menu_Screen->Get_Refresh_Button();
    Join_Server_Back_Button = Join_Server_Menu_Screen->Get_Back_Button();
    Join_Server_Lobby_List = Join_Server_Menu_Screen->Get_Lobby_List();
    Join_Server_Player_Name_Input = Join_Server_Menu_Screen->Get_Player_Name_Input();
}

void Game_Menues::Attach_Shared_Elements(Shared_Elements *sharedElements)
{
    // store shared state ref
    Shared_Group = sharedElements;
}

void Game_Menues::Attach_Networking_Elements(Game_Networking_Elements *networkingElements)
{
    // store networking ref
    Game_Networking_Group = networkingElements;
}

void Game_Menues::Attach_Multiplayer_Lobby_Elements(Game_Multiplayer_Lobby_Elements *lobbyElements)
{
    // store lobby screen ref
    Game_Multiplayer_Lobby_Group = lobbyElements;

    if(Game_Multiplayer_Lobby_Group != nullptr)
    {
        QWidget *Lobby_Widget = Game_Multiplayer_Lobby_Group->Get_Lobby_Widget();

        // only add the lobby screen once
        if(Screen_Stack->indexOf(Lobby_Widget) == -1)
            Screen_Stack->addWidget(Lobby_Widget);
    }
}

void Game_Menues::Setup_Navigation()
{
    if(Navigation_Is_Set_Up)
        return;

    // main menu -> multiplayer menu
    connect(Main_Menu_Multiplayer_Button, &QPushButton::clicked, this, [this]() {
        Go_To_Multiplayer_Menu();
    });

    // multiplayer menu -> main menu
    connect(Multiplayer_Back_Button, &QPushButton::clicked, this, [this]() {
        Go_To_Main_Menu();
    });

    // multiplayer menu -> host setup
    connect(Multiplayer_Host_Button, &QPushButton::clicked, this, [this]() {
        Go_To_Create_Lobby_Menu();
    });

    // multiplayer menu -> join screen
    connect(Multiplayer_Client_Button, &QPushButton::clicked, this, [this]() {
        Go_To_Join_Server_Menu();
    });

    // create lobby screen -> multiplayer menu
    connect(Create_Lobby_Back_Button, &QPushButton::clicked, this, [this]() {
        Go_To_Multiplayer_Menu();
    });

    // create lobby button starts the host flow
    connect(Create_Lobby_Button, &QPushButton::clicked, this, [this]() {
        Start_Hosting();
    });

    // join button starts the client flow
    connect(Join_Server_Join_Button, &QPushButton::clicked, this, [this]() {
        Join_Game();
    });

    // refresh button requests active lobbies
    connect(Join_Server_Refresh_Button, &QPushButton::clicked, this, [this]() {
        if(Shared_Group == nullptr || Game_Networking_Group == nullptr || Join_Server_Lobby_List == nullptr)
            return;

        // make sure the socket is listening first
        Game_Networking_Group->Start_Listening();

        // clear old results before a new scan
        Shared_Group->Clear_Discovered_Lobbies();
        Join_Server_Lobby_List->clear();

        Game_Networking_Group->Send_Discover_Lobbies();

        qDebug() << "Refreshing lobbies...";

        // wait a moment for host replies
        QTimer::singleShot(700, this, [this]() {
            if(Shared_Group == nullptr || Join_Server_Lobby_List == nullptr)
                return;

            Join_Server_Lobby_List->clear();

            QList<Lobby_Info> Lobbies = Shared_Group->Get_Discovered_Lobbies();

            // rebuild the visible lobby list
            for(int i = 0; i < Lobbies.size(); i++)
            {
                QString Lobby_Text = Lobbies[i].Lobby_Name + " | Host: " +
                                     Lobbies[i].Host_Name + " | " +
                                     QString::number(Lobbies[i].Current_Players) + "/" +
                                     QString::number(Lobbies[i].Max_Players);

                Join_Server_Lobby_List->addItem(Lobby_Text);
            }

            if(Join_Server_Lobby_List->count() > 0)
                Join_Server_Lobby_List->setCurrentRow(0);
        });
    });

    // join screen -> multiplayer menu
    connect(Join_Server_Back_Button, &QPushButton::clicked, this, [this]() {
        Go_To_Multiplayer_Menu();
    });

    Navigation_Is_Set_Up = true;
}

QWidget *Game_Menues::Get_Root_Widget() const
{
    return Screen_Stack;
}

void Game_Menues::Go_To_Multiplayer_Menu()
{
    // go to multiplayer select
    Screen_Stack->setCurrentWidget(Multiplayer_Menu_Widget);
}

void Game_Menues::Go_To_Main_Menu()
{
    // go back to main menu
    Screen_Stack->setCurrentWidget(Main_Menu_Widget);
}

void Game_Menues::Go_To_Create_Lobby_Menu()
{
    // open the host setup form
    Screen_Stack->setCurrentWidget(Create_Lobby_Menu_Widget);
}

void Game_Menues::Go_To_Join_Server_Menu()
{
    // open the lobby browser screen
    Screen_Stack->setCurrentWidget(Join_Server_Menu_Widget);
}

void Game_Menues::Go_To_Lobby_Menu()
{
    // switch into the lobby screen
    if(Game_Multiplayer_Lobby_Group != nullptr)
        Screen_Stack->setCurrentWidget(Game_Multiplayer_Lobby_Group->Get_Lobby_Widget());
}

void Game_Menues::Start_Hosting()
{
    // all linked systems need to exist first
    if(Shared_Group == nullptr || Game_Networking_Group == nullptr || Game_Multiplayer_Lobby_Group == nullptr)
    {
        qDebug() << "Missing groups";
        return;
    }

    // reset old lobby data and load the new form values
    Shared_Group->Reset_Lobby_State();
    Push_Create_Lobby_Form_Data_Into_Shared_Elements();

    // mark this machine as host
    Shared_Group->Set_Is_Hosting(true);
    Shared_Group->Set_Is_In_Lobby(true);
    Shared_Group->Set_Player_Role("Host");
    Shared_Group->Set_Connection_Status("Hosting lobby");
    Shared_Group->Set_Last_Network_Event("Host created lobby");

    Player_Info Host_Player;
    Host_Player.Player_ID = 1;
    Host_Player.Player_Name = Shared_Group->Get_Local_Player_Name();
    Host_Player.Is_Host = true;
    Host_Player.Is_Ready = true;

    // host is always player one in a new lobby
    Shared_Group->Clear_Players();
    Shared_Group->Add_Player(Host_Player);

    // start the socket and enter the lobby if it works
    if(Game_Networking_Group->Start_Listening())
    {
        qDebug() << "Hosting success";
        Game_Multiplayer_Lobby_Group->Refresh_From_Shared_Elements();
        Go_To_Lobby_Menu();
    }
    else
    {
        qDebug() << "Hosting failed";
        Game_Multiplayer_Lobby_Group->Refresh_From_Shared_Elements();
    }
}

void Game_Menues::Join_Game()
{
    // shared state, networking, and lobby ui all need to exist
    if(Shared_Group == nullptr || Game_Networking_Group == nullptr || Game_Multiplayer_Lobby_Group == nullptr)
    {
        qDebug() << "Missing groups";
        return;
    }

    // a lobby has to be selected first
    if(Join_Server_Lobby_List == nullptr || Join_Server_Lobby_List->currentItem() == nullptr)
    {
        qDebug() << "No lobby selected";
        return;
    }

    int Selected_Row = Join_Server_Lobby_List->currentRow();
    QList<Lobby_Info> Lobbies = Shared_Group->Get_Discovered_Lobbies();

    // guard against stale selection
    if(Selected_Row < 0 || Selected_Row >= Lobbies.size())
    {
        qDebug() << "Selected row out of range";
        return;
    }

    Lobby_Info Selected_Lobby = Lobbies[Selected_Row];

    // start from a clean lobby state
    Shared_Group->Reset_Lobby_State();

    QString Player_Name = "Player";
    if(Join_Server_Player_Name_Input != nullptr)
    {
        QString Entered_Name = Join_Server_Player_Name_Input->text().trimmed();

        // keep the default name if input is blank
        if(!Entered_Name.isEmpty())
            Player_Name = Entered_Name;
    }

    // store the selected lobby details
    Shared_Group->Set_Local_Player_Name(Player_Name);
    Shared_Group->Set_Selected_Lobby(Selected_Lobby);
    Shared_Group->Set_Lobby_Name(Selected_Lobby.Lobby_Name);
    Shared_Group->Set_Player_Role("Client");
    Shared_Group->Set_Is_Hosting(false);
    Shared_Group->Set_Is_In_Lobby(true);
    Shared_Group->Set_Remote_Address(Selected_Lobby.Host_Address.toString() + ":" +
                                     QString::number(Selected_Lobby.Host_Port));
    Shared_Group->Set_Last_Network_Event("Joining selected lobby");

    // start listening and send the join request
    Game_Networking_Group->Start_Listening();
    Game_Networking_Group->Request_To_Join(Selected_Lobby.Host_Address,
                                           Selected_Lobby.Host_Port);

    Game_Multiplayer_Lobby_Group->Refresh_From_Shared_Elements();
    Go_To_Lobby_Menu();
}

void Game_Menues::Push_Create_Lobby_Form_Data_Into_Shared_Elements()
{
    if(Shared_Group == nullptr)
        return;

    // copy form values into shared state
    Shared_Group->Set_Lobby_Name(Lobby_Name_Input->text());
    Shared_Group->Set_Max_Players(Max_Players_Input->value());
    Shared_Group->Set_Lobby_Privacy(Public_Lobby_Checkbox->isChecked());

    // set host defaults for a new lobby
    Shared_Group->Set_Is_In_Lobby(true);
    Shared_Group->Set_Local_Player_Name("Host");
    Shared_Group->Set_Local_Player_ID(1);
}
