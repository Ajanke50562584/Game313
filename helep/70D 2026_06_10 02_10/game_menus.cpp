#include "game_menus.h"

#include <QDebug>
#include <QTimer>

Game_Menus::Game_Menus(QWidget *parent)
    : QObject(parent)
{
    Screen_Stack = new QStackedWidget(parent); // stack that holds all menu/game screens

    Main_Menu_Screen = new Menu_Main_Screen(Screen_Stack); // main menu screen
    Multiplayer_Menu_Screen = new Menu_Multiplayer_Screen(Screen_Stack); // multiplayer choice screen
    Create_Lobby_Menu_Screen = new Menu_Create_Lobby_Screen(Screen_Stack); // create lobby screen
    Join_Server_Menu_Screen = new Menu_Join_Server_Screen(Screen_Stack); // join server screen
    Story_Mode_Main_Menu_Screen = new Story_Mode_Main_Screen(Screen_Stack); // story mode menu screen
    Single_Player_Game_Screen_Widget = new Single_Player_Game_Screen(Screen_Stack); // OpenGL single player game screen
    Multiplayer_Game_Screen_Widget = new Multiplayer_Game_Screen(Screen_Stack); // OpenGL multiplayer game screen
    Leaderboard_Menu_Screen = new Leaderboard_Screen(Screen_Stack); // leaderboard screen

    Main_Menu_Widget = Main_Menu_Screen; // store widget pointer for screen switching
    Multiplayer_Menu_Widget = Multiplayer_Menu_Screen; // store widget pointer for screen switching
    Create_Lobby_Menu_Widget = Create_Lobby_Menu_Screen; // store widget pointer for screen switching
    Join_Server_Menu_Widget = Join_Server_Menu_Screen; // store widget pointer for screen switching

    Screen_Stack->addWidget(Main_Menu_Widget); // add main menu to stack
    Screen_Stack->addWidget(Multiplayer_Menu_Widget); // add multiplayer menu to stack
    Screen_Stack->addWidget(Create_Lobby_Menu_Widget); // add create lobby screen to stack
    Screen_Stack->addWidget(Join_Server_Menu_Widget); // add join screen to stack
    Screen_Stack->addWidget(Story_Mode_Main_Menu_Screen); // add story mode menu to stack
    Screen_Stack->addWidget(Single_Player_Game_Screen_Widget); // add OpenGL single player game to stack
    Screen_Stack->addWidget(Multiplayer_Game_Screen_Widget); // add OpenGL multiplayer game to stack
    Screen_Stack->addWidget(Leaderboard_Menu_Screen); // add leaderboard screen

    Screen_Stack->setCurrentWidget(Main_Menu_Widget); // start on the main menu

    // Cache buttons and form fields to connect navigation and read user input.
    Main_Menu_Multiplayer_Button = Main_Menu_Screen->Get_Multiplayer_Button();
    Main_Menu_Story_Mode_Button = Main_Menu_Screen->Get_Story_Mode_Button();
    Main_Menu_Highscores_Button = Main_Menu_Screen->Get_Highscores_Button();

    Multiplayer_Host_Button = Multiplayer_Menu_Screen->Get_Host_Button();
    Multiplayer_Client_Button = Multiplayer_Menu_Screen->Get_Client_Button();
    Multiplayer_Back_Button = Multiplayer_Menu_Screen->Get_Back_Button();

    Create_Lobby_Button = Create_Lobby_Menu_Screen->Get_Create_Lobby_Button();
    Create_Lobby_Back_Button = Create_Lobby_Menu_Screen->Get_Back_Button();
    Lobby_Name_Input = Create_Lobby_Menu_Screen->Get_Lobby_Name_Input();
    Host_Name_Input = Create_Lobby_Menu_Screen->Get_Host_Name_Input();
    Max_Players_Input = Create_Lobby_Menu_Screen->Get_Max_Players_Input();
    Public_Lobby_Checkbox = Create_Lobby_Menu_Screen->Get_Public_Lobby_Checkbox();

    Join_Server_Join_Button = Join_Server_Menu_Screen->Get_Join_Button();
    Join_Server_Refresh_Button = Join_Server_Menu_Screen->Get_Refresh_Button();
    Join_Server_Back_Button = Join_Server_Menu_Screen->Get_Back_Button();
    Join_Server_Lobby_List = Join_Server_Menu_Screen->Get_Lobby_List();
    Join_Server_Player_Name_Input = Join_Server_Menu_Screen->Get_Player_Name_Input();

    Tutorial_Button = Story_Mode_Main_Menu_Screen->Get_Tutorial_Button();
    Select_Level_Button = Story_Mode_Main_Menu_Screen->Get_Select_Level_Button();
    Level_Select_Back_Button = Story_Mode_Main_Menu_Screen->Get_Back_Button();
    Leaderboard_Back_Button = Leaderboard_Menu_Screen->Get_Back_Button();

    Shared_Group = nullptr; // shared state will be attached later
    Game_Networking_Group = nullptr; // networking will be attached later
    Game_Multiplayer_Lobby_Group = nullptr; // lobby controller will be attached later

    Navigation_Is_Set_Up = false; // navigation connections have not been created yet
}

void Game_Menus::Cache_Screen_Controls()
{
    // get the controls out of each screen
    Main_Menu_Multiplayer_Button = Main_Menu_Screen->Get_Multiplayer_Button();
    Main_Menu_Story_Mode_Button = Main_Menu_Screen->Get_Story_Mode_Button();
    Main_Menu_Highscores_Button = Main_Menu_Screen->Get_Highscores_Button();
    Multiplayer_Host_Button = Multiplayer_Menu_Screen->Get_Host_Button();
    Multiplayer_Client_Button = Multiplayer_Menu_Screen->Get_Client_Button();
    Multiplayer_Back_Button = Multiplayer_Menu_Screen->Get_Back_Button();

    Create_Lobby_Button = Create_Lobby_Menu_Screen->Get_Create_Lobby_Button();
    Create_Lobby_Back_Button = Create_Lobby_Menu_Screen->Get_Back_Button();
    Lobby_Name_Input = Create_Lobby_Menu_Screen->Get_Lobby_Name_Input();
    Host_Name_Input = Create_Lobby_Menu_Screen->Get_Host_Name_Input();
    Max_Players_Input = Create_Lobby_Menu_Screen->Get_Max_Players_Input();
    Public_Lobby_Checkbox = Create_Lobby_Menu_Screen->Get_Public_Lobby_Checkbox();

    Join_Server_Join_Button = Join_Server_Menu_Screen->Get_Join_Button();
    Join_Server_Refresh_Button = Join_Server_Menu_Screen->Get_Refresh_Button();
    Join_Server_Back_Button = Join_Server_Menu_Screen->Get_Back_Button();
    Join_Server_Lobby_List = Join_Server_Menu_Screen->Get_Lobby_List();
    Join_Server_Player_Name_Input = Join_Server_Menu_Screen->Get_Player_Name_Input();

    Tutorial_Button = Story_Mode_Main_Menu_Screen->Get_Tutorial_Button();
    Select_Level_Button = Story_Mode_Main_Menu_Screen->Get_Select_Level_Button();
    Level_Select_Back_Button = Story_Mode_Main_Menu_Screen->Get_Back_Button();
    Leaderboard_Back_Button = Leaderboard_Menu_Screen->Get_Back_Button();
}

void Game_Menus::Attach_Shared_Elements(Shared_Elements *sharedElements)
{
    Shared_Group = sharedElements;
}

void Game_Menus::Attach_Networking_Elements(Game_Networking_Elements *networkingElements)
{
    Game_Networking_Group = networkingElements;
    connect(Game_Networking_Group, SIGNAL(Game_Start_Received()),this, SLOT(Go_To_Multiplayer_Game()));
}

void Game_Menus::Attach_Multiplayer_Lobby_Elements(Game_Multiplayer_Lobby_Elements *lobbyElements)
{
    Game_Multiplayer_Lobby_Group = lobbyElements;

    if(Game_Multiplayer_Lobby_Group != nullptr)
    {
        QWidget *Lobby_Widget = Game_Multiplayer_Lobby_Group->Get_Lobby_Widget();

        // only add the lobby screen once
        if(Screen_Stack->indexOf(Lobby_Widget) == -1) // -1 Index i.e. is not yet on stack
            Screen_Stack->addWidget(Lobby_Widget);

        connect(Game_Multiplayer_Lobby_Group, SIGNAL(Leave_Lobby_Requested()),this, SLOT(Go_To_Multiplayer_Menu()));
    }
}

void Game_Menus::Setup_Navigation()
{
    if(Navigation_Is_Set_Up)
        return;

    //Connecting all the nav buttosn
    connect(Main_Menu_Multiplayer_Button, SIGNAL(clicked()), this, SLOT(Main_Menu_Multiplayer_Button_Clicked()));
    connect(Multiplayer_Back_Button, SIGNAL(clicked()), this, SLOT(Multiplayer_Back_Button_Clicked()));
    connect(Multiplayer_Host_Button, SIGNAL(clicked()), this, SLOT(Multiplayer_Host_Button_Clicked()));
    connect(Multiplayer_Client_Button, SIGNAL(clicked()), this, SLOT(Multiplayer_Client_Button_Clicked()));
    connect(Create_Lobby_Back_Button, SIGNAL(clicked()), this, SLOT(Create_Lobby_Back_Button_Clicked()));
    connect(Create_Lobby_Button, SIGNAL(clicked()), this, SLOT(Create_Lobby_Button_Clicked()));
    connect(Join_Server_Join_Button, SIGNAL(clicked()), this, SLOT(Join_Server_Join_Button_Clicked()));
    connect(Join_Server_Refresh_Button, SIGNAL(clicked()), this, SLOT(Join_Server_Refresh_Button_Clicked()));
    connect(Join_Server_Back_Button, SIGNAL(clicked()), this, SLOT(Join_Server_Back_Button_Clicked()));
    connect(Main_Menu_Story_Mode_Button, SIGNAL(clicked()), this, SLOT(Main_Menu_Story_Mode_Button_Clicked()));
    connect(Main_Menu_Highscores_Button, SIGNAL(clicked()), this, SLOT(Highscores_Button_Clicked()));
    connect(Tutorial_Button, SIGNAL(clicked()), this, SLOT(Tutorial_Button_Clicked()));
    connect(Select_Level_Button, SIGNAL(clicked()), this, SLOT(Select_Level_Button_Clicked()));
    connect(Level_Select_Back_Button, SIGNAL(clicked()), this, SLOT(Level_Select_Back_Clicked()));
    connect(Leaderboard_Back_Button, SIGNAL(clicked()), this, SLOT(Leaderboard_Back_Clicked()));
    // multiplayer lobby starts the multiplayer game screen
    if(Game_Multiplayer_Lobby_Group != nullptr)
        connect(Game_Multiplayer_Lobby_Group, SIGNAL(Start_Game_Requested()),this, SLOT(Go_To_Multiplayer_Game()));

    Navigation_Is_Set_Up = true;
}

QWidget *Game_Menus::Get_Root_Widget() const // Returns a ptr to the screen stack
{
    return Screen_Stack;
}

bool Game_Menus::Is_Single_Player_Game_Current() const
{
    return Screen_Stack->currentWidget() == Single_Player_Game_Screen_Widget;
}

bool Game_Menus::Is_Multiplayer_Game_Current() const
{
    return Screen_Stack->currentWidget() == Multiplayer_Game_Screen_Widget;
}

Single_Player_Game_Screen *Game_Menus::Get_Single_Player_Game_Screen() const
{
    return Single_Player_Game_Screen_Widget;
}

Multiplayer_Game_Screen *Game_Menus::Get_Multiplayer_Game_Screen() const
{
    return Multiplayer_Game_Screen_Widget;
}

void Game_Menus::Go_To_Multiplayer_Menu()
{
    // go to the multiplayer screen
    Screen_Stack->setCurrentWidget(Multiplayer_Menu_Widget);
}

void Game_Menus::Go_To_Main_Menu()
{
    // go back to the main menu
    Screen_Stack->setCurrentWidget(Main_Menu_Widget);
}

void Game_Menus::Go_To_Create_Lobby_Menu()
{
    // open the create lobby screen
    Screen_Stack->setCurrentWidget(Create_Lobby_Menu_Widget);
}

void Game_Menus::Go_To_Join_Server_Menu()
{
    // open the join screen
    Screen_Stack->setCurrentWidget(Join_Server_Menu_Widget);
}

void Game_Menus::Go_To_Lobby_Menu()
{
    // go into the lobby screen
    if(Game_Multiplayer_Lobby_Group != nullptr)
        Screen_Stack->setCurrentWidget(Game_Multiplayer_Lobby_Group->Get_Lobby_Widget());
}

void Game_Menus::Go_To_Story_Mode()
{
    Screen_Stack->setCurrentWidget(Story_Mode_Main_Menu_Screen);
}

void Game_Menus::Go_To_Leaderboard()
{
    Leaderboard_Menu_Screen->Refresh_Leaderboard();
    Screen_Stack->setCurrentWidget(Leaderboard_Menu_Screen);
}

void Game_Menus::Go_To_Single_Player_Game()
{
    Screen_Stack->setCurrentWidget(Single_Player_Game_Screen_Widget);
    Single_Player_Game_Screen_Widget->setFocus(Qt::OtherFocusReason);
}

void Game_Menus::Go_To_Multiplayer_Game()
{
    Screen_Stack->setCurrentWidget(Multiplayer_Game_Screen_Widget);
    Multiplayer_Game_Screen_Widget->setFocus(Qt::OtherFocusReason);
}

void Game_Menus::Start_Hosting()
{
    if(Host_Name_Input == nullptr || Host_Name_Input->text().trimmed().isEmpty())
    {
        if(Host_Name_Input != nullptr)
            Host_Name_Input->setFocus();
        return;
    }

    // hosting needs these classes
    if(Shared_Group == nullptr || Game_Networking_Group == nullptr || Game_Multiplayer_Lobby_Group == nullptr)
    {
        qDebug() << "Missing groups";
        return;
    }

    // clear old lobby data then load the new form data
    Shared_Group->Reset_Lobby_State();
    Push_Create_Lobby_Form_Data_Into_Shared_Elements();

    // Set local as host
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

void Game_Menus::Join_Game()
{
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
            Player_Name = Entered_Name;
    }

    // save the chosen lobby details into shared data
    Shared_Group->Set_Local_Player_Name(Player_Name);
    Shared_Group->Set_Selected_Lobby(Selected_Lobby);
    Shared_Group->Set_Lobby_Name(Selected_Lobby.Lobby_Name);
    Shared_Group->Set_Player_Role("Client");
    Shared_Group->Set_Is_Hosting(false);
    Shared_Group->Set_Is_In_Lobby(true);
    Shared_Group->Set_Remote_Address(Selected_Lobby.Host_Address.toString() + ":" + QString::number(Selected_Lobby.Host_Port));
    Shared_Group->Set_Last_Network_Event("Joining selected lobby");

    // start listening then send the join request
    Game_Networking_Group->Start_Client_Listening();
    Game_Networking_Group->Request_To_Join(Selected_Lobby.Host_Address,Selected_Lobby.Host_Port);

    Game_Multiplayer_Lobby_Group->Refresh_From_Shared_Elements();
    Go_To_Lobby_Menu();
}

void Game_Menus::Push_Create_Lobby_Form_Data_Into_Shared_Elements()
{
    if(Shared_Group == nullptr)
        return;
    // copy the form values into shared data
    Shared_Group->Set_Lobby_Name(Lobby_Name_Input->text());
    Shared_Group->Set_Max_Players(Max_Players_Input->value());
    Shared_Group->Set_Lobby_Privacy(Public_Lobby_Checkbox->isChecked());

    // default host details
    Shared_Group->Set_Is_In_Lobby(true);
    Shared_Group->Set_Local_Player_Name(Host_Name_Input != nullptr ? Host_Name_Input->text() : "Player");
    Shared_Group->Set_Local_Player_ID(1);
}

void Game_Menus::Main_Menu_Story_Mode_Button_Clicked()
{
    Go_To_Story_Mode();
}

void Game_Menus::Main_Menu_Multiplayer_Button_Clicked()
{
    Go_To_Multiplayer_Menu();
}

void Game_Menus::Highscores_Button_Clicked()
{
    Go_To_Leaderboard();
}

void Game_Menus::Leaderboard_Back_Clicked()
{
    Go_To_Main_Menu();
}

void Game_Menus::Multiplayer_Back_Button_Clicked()
{
    Go_To_Main_Menu();
}

void Game_Menus::Multiplayer_Host_Button_Clicked()
{
    Go_To_Create_Lobby_Menu();
}

void Game_Menus::Multiplayer_Client_Button_Clicked()
{
    Go_To_Join_Server_Menu();
}

void Game_Menus::Create_Lobby_Back_Button_Clicked()
{
    Go_To_Multiplayer_Menu();
}

void Game_Menus::Create_Lobby_Button_Clicked()
{
    Start_Hosting();
}

void Game_Menus::Join_Server_Join_Button_Clicked()
{
    Join_Game();
}

void Game_Menus::Join_Server_Refresh_Button_Clicked()
{
    if(Shared_Group == nullptr || Game_Networking_Group == nullptr || Join_Server_Lobby_List == nullptr)
        return;

    // make sure the socket is open first
    Game_Networking_Group->Start_Client_Listening();

    // clear the old lobbies before looking again
    Shared_Group->Clear_Discovered_Lobbies();
    Join_Server_Lobby_List->clear();

    Game_Networking_Group->Send_Discover_Lobbies();

    qDebug() << "Refreshing lobbies...";

    // wait a little so host machines can reply
    QTimer::singleShot(1000, this, SLOT(Refresh_Discovered_Lobbies()));
}

void Game_Menus::Join_Server_Back_Button_Clicked()
{
    Go_To_Multiplayer_Menu();
}

void Game_Menus::Tutorial_Button_Clicked()
{
    Single_Player_Game_Screen_Widget->Start_Single_Player_Level(true);
    Go_To_Single_Player_Game();
}

void Game_Menus::Select_Level_Button_Clicked()
{
    qDebug() << "Select level clicked";
    Single_Player_Game_Screen_Widget->Start_Single_Player_Level(false);
    Go_To_Single_Player_Game();
}

void Game_Menus::Level_Select_Back_Clicked()
{
    Go_To_Main_Menu();
}

void Game_Menus::Refresh_Discovered_Lobbies()
{
    if(Shared_Group == nullptr || Join_Server_Lobby_List == nullptr)
        return;

    Join_Server_Lobby_List->clear();

    QList<Lobby_Info> Lobbies = Shared_Group->Get_Discovered_Lobbies();

    // rebuild the list on screen
    for(int i = 0; i < Lobbies.size(); i++)
    {
        QString Lobby_Text = Lobbies[i].Lobby_Name + " | Host: " + Lobbies[i].Host_Name + " | " + QString::number(Lobbies[i].Current_Players) + "/" + QString::number(Lobbies[i].Max_Players);

        Join_Server_Lobby_List->addItem(Lobby_Text);
    }

    if(Join_Server_Lobby_List->count() > 0)
        Join_Server_Lobby_List->setCurrentRow(0);
}
