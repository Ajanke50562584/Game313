#include "game_menues.h"

#include <QDebug>
#include <QTimer>

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
    // create the menu stack
    Build_Screens(parent);

    // store the controls we need later
    Cache_Screen_Controls();
}

void Game_Menues::Build_Screens(QWidget *parent)
{
    // this stack holds all the menu screens
    Screen_Stack = new QStackedWidget(parent);

    // make each screen
    Main_Menu_Screen = new Menu_Main_Screen(Screen_Stack);
    Multiplayer_Menu_Screen = new Menu_Multiplayer_Screen(Screen_Stack);
    Create_Lobby_Menu_Screen = new Menu_Create_Lobby_Screen(Screen_Stack);
    Join_Server_Menu_Screen = new Menu_Join_Server_Screen(Screen_Stack);

    // keep widget refs for screen switching
    Main_Menu_Widget = Main_Menu_Screen;
    Multiplayer_Menu_Widget = Multiplayer_Menu_Screen;
    Create_Lobby_Menu_Widget = Create_Lobby_Menu_Screen;
    Join_Server_Menu_Widget = Join_Server_Menu_Screen;

    // put them into the stack
    Screen_Stack->addWidget(Main_Menu_Widget);
    Screen_Stack->addWidget(Multiplayer_Menu_Widget);
    Screen_Stack->addWidget(Create_Lobby_Menu_Widget);
    Screen_Stack->addWidget(Join_Server_Menu_Widget);

    // start on the main menu
    Screen_Stack->setCurrentWidget(Main_Menu_Widget);
}

void Game_Menues::Cache_Screen_Controls()
{
    // get the controls out of each screen
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
    // keep the shared data
    Shared_Group = sharedElements;
}

void Game_Menues::Attach_Networking_Elements(Game_Networking_Elements *networkingElements)
{
    // keep the networking class
    Game_Networking_Group = networkingElements;
}

void Game_Menues::Attach_Multiplayer_Lobby_Elements(Game_Multiplayer_Lobby_Elements *lobbyElements)
{
    // keep the lobby screen
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


    connect(Main_Menu_Multiplayer_Button, SIGNAL(clicked()), this, SLOT(Main_Menu_Multiplayer_Button_Clicked()));
    connect(Multiplayer_Back_Button, SIGNAL(clicked()), this, SLOT(Multiplayer_Back_Button_Clicked()));
    connect(Multiplayer_Host_Button, SIGNAL(clicked()), this, SLOT(Multiplayer_Host_Button_Clicked()));
    connect(Multiplayer_Client_Button, SIGNAL(clicked()), this, SLOT(Multiplayer_Client_Button_Clicked()));
    connect(Create_Lobby_Back_Button, SIGNAL(clicked()), this, SLOT(Create_Lobby_Back_Button_Clicked()));
    connect(Create_Lobby_Button, SIGNAL(clicked()), this, SLOT(Create_Lobby_Button_Clicked()));
    connect(Join_Server_Join_Button, SIGNAL(clicked()), this, SLOT(Join_Server_Join_Button_Clicked()));
    connect(Join_Server_Refresh_Button, SIGNAL(clicked()), this, SLOT(Join_Server_Refresh_Button_Clicked()));
    connect(Join_Server_Back_Button, SIGNAL(clicked()), this, SLOT(Join_Server_Back_Button_Clicked()));

    Navigation_Is_Set_Up = true;
}

QWidget *Game_Menues::Get_Root_Widget() const
{
    return Screen_Stack;
}

void Game_Menues::Go_To_Multiplayer_Menu()
{
    // go to the multiplayer screen
    Screen_Stack->setCurrentWidget(Multiplayer_Menu_Widget);
}

void Game_Menues::Go_To_Main_Menu()
{
    // go back to the main menu
    Screen_Stack->setCurrentWidget(Main_Menu_Widget);
}

void Game_Menues::Go_To_Create_Lobby_Menu()
{
    // open the create lobby screen
    Screen_Stack->setCurrentWidget(Create_Lobby_Menu_Widget);
}

void Game_Menues::Go_To_Join_Server_Menu()
{
    // open the join screen
    Screen_Stack->setCurrentWidget(Join_Server_Menu_Widget);
}

void Game_Menues::Go_To_Lobby_Menu()
{
    // go into the lobby screen
    if(Game_Multiplayer_Lobby_Group != nullptr)
        Screen_Stack->setCurrentWidget(Game_Multiplayer_Lobby_Group->Get_Lobby_Widget());
}

void Game_Menues::Start_Hosting()
{
    // hosting needs these classes
    if(Shared_Group == nullptr || Game_Networking_Group == nullptr || Game_Multiplayer_Lobby_Group == nullptr)
    {
        qDebug() << "Missing groups";
        return;
    }

    // clear old lobby data then load the new form data
    Shared_Group->Reset_Lobby_State();
    Push_Create_Lobby_Form_Data_Into_Shared_Elements();

    // this machine is now the host
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

    // host is always player one
    Shared_Group->Clear_Players();
    Shared_Group->Add_Player(Host_Player);

    // start listening and go into the lobby if it works
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
    // joining needs these classes
    if(Shared_Group == nullptr || Game_Networking_Group == nullptr || Game_Multiplayer_Lobby_Group == nullptr)
    {
        qDebug() << "Missing groups";
        return;
    }

    // player must select a lobby first
    if(Join_Server_Lobby_List == nullptr || Join_Server_Lobby_List->currentItem() == nullptr)
    {
        qDebug() << "No lobby selected";
        return;
    }

    int Selected_Row = Join_Server_Lobby_List->currentRow();
    QList<Lobby_Info> Lobbies = Shared_Group->Get_Discovered_Lobbies();

    // make sure the selected row is valid
    if(Selected_Row < 0 || Selected_Row >= Lobbies.size())
    {
        qDebug() << "Selected row out of range";
        return;
    }

    Lobby_Info Selected_Lobby = Lobbies[Selected_Row];

    // clear old lobby data first
    Shared_Group->Reset_Lobby_State();

    QString Player_Name = "Player";
    if(Join_Server_Player_Name_Input != nullptr)
    {
        QString Entered_Name = Join_Server_Player_Name_Input->text().trimmed();

        // keep the default name if input is blank
        if(!Entered_Name.isEmpty())
            Player_Name = "The void knows not your name";
    }

    // save the chosen lobby details into shared data
    Shared_Group->Set_Local_Player_Name(Player_Name);
    Shared_Group->Set_Selected_Lobby(Selected_Lobby);
    Shared_Group->Set_Lobby_Name(Selected_Lobby.Lobby_Name);
    Shared_Group->Set_Player_Role("Client");
    Shared_Group->Set_Is_Hosting(false);
    Shared_Group->Set_Is_In_Lobby(true);
    Shared_Group->Set_Remote_Address(Selected_Lobby.Host_Address.toString() + ":" +
                                     QString::number(Selected_Lobby.Host_Port));
    Shared_Group->Set_Last_Network_Event("Joining selected lobby");

    // start listening then send the join request
    Game_Networking_Group->Start_Listening();
    Game_Networking_Group->Request_To_Join(Selected_Lobby.Host_Address,Selected_Lobby.Host_Port);

    Game_Multiplayer_Lobby_Group->Refresh_From_Shared_Elements();
    Go_To_Lobby_Menu();
}

void Game_Menues::Push_Create_Lobby_Form_Data_Into_Shared_Elements()
{
    if(Shared_Group == nullptr)
        return;

    // copy the form values into shared data
    Shared_Group->Set_Lobby_Name(Lobby_Name_Input->text());
    Shared_Group->Set_Max_Players(Max_Players_Input->value());
    Shared_Group->Set_Lobby_Privacy(Public_Lobby_Checkbox->isChecked());

    // default host details
    Shared_Group->Set_Is_In_Lobby(true);
    Shared_Group->Set_Local_Player_Name("The Last Constellation");
    Shared_Group->Set_Local_Player_ID(1);
}

void Game_Menues::Main_Menu_Multiplayer_Button_Clicked()
{
    Go_To_Multiplayer_Menu();
}

void Game_Menues::Multiplayer_Back_Button_Clicked()
{
    Go_To_Main_Menu();
}

void Game_Menues::Multiplayer_Host_Button_Clicked()
{
    Go_To_Create_Lobby_Menu();
}

void Game_Menues::Multiplayer_Client_Button_Clicked()
{
    Go_To_Join_Server_Menu();
}

void Game_Menues::Create_Lobby_Back_Button_Clicked()
{
    Go_To_Multiplayer_Menu();
}

void Game_Menues::Create_Lobby_Button_Clicked()
{
    Start_Hosting();
}

void Game_Menues::Join_Server_Join_Button_Clicked()
{
    Join_Game();
}

void Game_Menues::Join_Server_Refresh_Button_Clicked()
{
    if(Shared_Group == nullptr || Game_Networking_Group == nullptr || Join_Server_Lobby_List == nullptr)
        return;

    // make sure the socket is open first
    Game_Networking_Group->Start_Listening();

    // clear the old lobbies before looking again
    Shared_Group->Clear_Discovered_Lobbies();
    Join_Server_Lobby_List->clear();

    Game_Networking_Group->Send_Discover_Lobbies();

    qDebug() << "Refreshing lobbies...";

    // wait a little so host machines can reply
    QTimer::singleShot(1000, this, SLOT(Refresh_Discovered_Lobbies()));
}

void Game_Menues::Join_Server_Back_Button_Clicked()
{
    Go_To_Multiplayer_Menu();
}

void Game_Menues::Refresh_Discovered_Lobbies()
{
    if(Shared_Group == nullptr || Join_Server_Lobby_List == nullptr)
        return;

    Join_Server_Lobby_List->clear();

    QList<Lobby_Info> Lobbies = Shared_Group->Get_Discovered_Lobbies();

    // rebuild the list on screen
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
}
