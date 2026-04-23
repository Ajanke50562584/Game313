#include "game_multiplayer_lobby_elements.h"
#include "shared_elements.h"

#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QSizePolicy>
#include <QTextEdit>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

Game_Multiplayer_Lobby_Elements::Game_Multiplayer_Lobby_Elements(QWidget *parent)
    : QObject(parent)
    , Lobby_Widget(new QWidget(parent))
    , Shared_Group(nullptr)
    , Ellipsis_Timer(nullptr)
    , Ellipsis_Frame(0)
{
    // overall style
    Lobby_Widget->setStyleSheet(
        "QWidget {"
        "background: transparent;"
        "color: rgb(28, 36, 58);"
        "font-weight: bold;"
        "}"
        "QLabel {"
        "background: transparent;"
        "border: none;"
        "color: rgb(28, 36, 58);"
        "}"
        "QPushButton {"
        "font-weight: bold;"
        "}"
        "QTextEdit, QLineEdit {"
        "background-color: rgba(255, 255, 255, 185);"
        "color: rgb(28, 36, 58);"
        "border: 1px solid rgba(120,150,210,170);"
        "border-radius: 12px;"
        "padding: 8px;"
        "}"
        "QScrollArea {"
        "background: transparent;"
        "border: none;"
        "}"
    );

    // button style
    const QString Lobby_Button_Style =
        "QPushButton {"
        "background-color: rgba(18, 32, 60, 220);"
        "color: rgb(245,245,250);"
        "border: 2px solid rgba(110,145,210,210);"
        "border-radius: 18px;"
        "padding: 10px 18px;"
        "font-size: 16px;"
        "font-weight: 700;"
        "min-height: 42px;"
        "}"
        "QPushButton:hover {"
        "background-color: rgba(32, 52, 92, 235);"
        "}"
        "QPushButton:pressed {"
        "background-color: rgba(12, 22, 42, 245);"
        "}";

    // main grid
    QGridLayout *Main_Layout = new QGridLayout(Lobby_Widget);
    Main_Layout->setContentsMargins(24, 24, 24, 24);
    Main_Layout->setHorizontalSpacing(20);
    Main_Layout->setVerticalSpacing(20);

    Top_Left_Panel = new QWidget(Lobby_Widget);
    Top_Middle_Panel = new QWidget(Lobby_Widget);
    Top_Right_Panel = new QWidget(Lobby_Widget);
    Bottom_Left_Panel = new QWidget(Lobby_Widget);
    Bottom_Right_Panel = new QWidget(Lobby_Widget);

    Top_Left_Panel->setStyleSheet("background: transparent; border: none;");
    Top_Middle_Panel->setStyleSheet("background: transparent; border: none;");
    Top_Right_Panel->setStyleSheet("background: transparent; border: none;");
    Bottom_Left_Panel->setStyleSheet("background: transparent; border: none;");
    Bottom_Right_Panel->setStyleSheet("background: transparent; border: none;");

    Main_Layout->addWidget(Top_Left_Panel, 0, 0);
    Main_Layout->addWidget(Top_Middle_Panel, 0, 1);
    Main_Layout->addWidget(Top_Right_Panel, 0, 2);
    Main_Layout->addWidget(Bottom_Left_Panel, 1, 0, 1, 2);
    Main_Layout->addWidget(Bottom_Right_Panel, 1, 2);

    Main_Layout->setRowStretch(0, 3);
    Main_Layout->setRowStretch(1, 2);
    Main_Layout->setColumnStretch(0, 3);
    Main_Layout->setColumnStretch(1, 4);
    Main_Layout->setColumnStretch(2, 3);

    // =========================
    // top left : player list
    // =========================
    QVBoxLayout *Top_Left_Layout = new QVBoxLayout(Top_Left_Panel);
    Top_Left_Layout->setContentsMargins(0, 0, 0, 0);
    Top_Left_Layout->setSpacing(8);

    QLabel *Player_List_Title = new QLabel("Players", Top_Left_Panel);
    Player_List_Title->setStyleSheet(
        "font-size: 24px;"
        "font-weight: 700;"
        "color: rgb(28, 36, 58);");

    Player_List_Scroll_Area = new QScrollArea(Top_Left_Panel);
    Player_List_Scroll_Area->setWidgetResizable(true);
    Player_List_Scroll_Area->setFrameShape(QFrame::NoFrame);
    Player_List_Scroll_Area->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    Player_List_Scroll_Area->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    Player_List_Container = new QWidget();
    Player_List_Container->setStyleSheet("background: transparent; border: none;");

    Player_List_Layout = new QVBoxLayout(Player_List_Container);
    Player_List_Layout->setContentsMargins(0, 0, 0, 0);
    Player_List_Layout->setSpacing(6);

    // start empty
    Player_List_Layout->addStretch();

    Player_List_Scroll_Area->setWidget(Player_List_Container);

    Top_Left_Layout->addWidget(Player_List_Title);
    Top_Left_Layout->addWidget(Player_List_Scroll_Area);

    // =========================
    // top middle : lobby info
    // =========================
    QVBoxLayout *Top_Middle_Layout = new QVBoxLayout(Top_Middle_Panel);
    Top_Middle_Layout->setContentsMargins(0, 0, 0, 0);

    Lobby_Name_Label = new QLabel("The Last Constellation Lobby", Top_Middle_Panel);
    Connection_Status_Label = new QLabel("Connection Status: Idle", Top_Middle_Panel);

    Lobby_Name_Label->setAlignment(Qt::AlignCenter);
    Connection_Status_Label->setAlignment(Qt::AlignCenter);

    Lobby_Name_Label->setStyleSheet(
        "font-size: 30px;"
        "font-weight: 700;"
        "color: rgb(30, 38, 62);"
        "background-color: rgba(255,255,255,130);"
        "border-radius: 18px;"
        "padding: 8px 16px;");

    Connection_Status_Label->setStyleSheet(
        "font-size: 18px;"
        "font-weight: 600;"
        "color: rgb(40, 50, 78);"
        "background-color: rgba(255,255,255,120);"
        "border-radius: 12px;"
        "padding: 6px 12px;");

    Top_Middle_Layout->addStretch();
    Top_Middle_Layout->addWidget(Lobby_Name_Label);
    Top_Middle_Layout->addSpacing(12);
    Top_Middle_Layout->addWidget(Connection_Status_Label);
    Top_Middle_Layout->addStretch();

    // =========================
    // top right : host controls
    // =========================
    QVBoxLayout *Top_Right_Layout = new QVBoxLayout(Top_Right_Panel);
    Top_Right_Layout->setContentsMargins(0, 0, 0, 0);
    Top_Right_Layout->setSpacing(10);

    Host_Options_Title_Label = new QLabel("Host Controls", Top_Right_Panel);
    Host_Options_Title_Label->setStyleSheet(
        "font-size: 24px;"
        "font-weight: 700;"
        "color: rgb(28, 36, 58);");

    Start_Game_Button = new QPushButton("Start Game", Top_Right_Panel);
    Edit_Lobby_Settings_Button = new QPushButton("Edit Lobby Settings", Top_Right_Panel);
    Kick_Player_Button = new QPushButton("Kick Player", Top_Right_Panel);

    Start_Game_Button->setStyleSheet(Lobby_Button_Style);
    Edit_Lobby_Settings_Button->setStyleSheet(Lobby_Button_Style);
    Kick_Player_Button->setStyleSheet(Lobby_Button_Style);

    Top_Right_Layout->addWidget(Host_Options_Title_Label);
    Top_Right_Layout->addWidget(Start_Game_Button);
    Top_Right_Layout->addWidget(Edit_Lobby_Settings_Button);
    Top_Right_Layout->addWidget(Kick_Player_Button);
    Top_Right_Layout->addStretch();

    // =========================
    // bottom left : buttons
    // =========================
    QHBoxLayout *Bottom_Left_Layout = new QHBoxLayout(Bottom_Left_Panel);
    Bottom_Left_Layout->setContentsMargins(0, 0, 0, 0);
    Bottom_Left_Layout->setSpacing(12);

    Leave_Lobby_Button = new QPushButton("Leave Lobby", Bottom_Left_Panel);
    Customize_Character_Button = new QPushButton("Customize Character", Bottom_Left_Panel);
    Vote_Map_Button = new QPushButton("Vote Map", Bottom_Left_Panel);

    Leave_Lobby_Button->setStyleSheet(Lobby_Button_Style);
    Customize_Character_Button->setStyleSheet(Lobby_Button_Style);
    Vote_Map_Button->setStyleSheet(Lobby_Button_Style);

    Leave_Lobby_Button->setMinimumWidth(170);
    Customize_Character_Button->setMinimumWidth(220);
    Vote_Map_Button->setMinimumWidth(170);

    Bottom_Left_Layout->addWidget(Leave_Lobby_Button);
    Bottom_Left_Layout->addWidget(Customize_Character_Button);
    Bottom_Left_Layout->addWidget(Vote_Map_Button);
    Bottom_Left_Layout->addStretch();

    // =========================
    // bottom right : chat
    // =========================
    QVBoxLayout *Bottom_Right_Layout = new QVBoxLayout(Bottom_Right_Panel);
    Bottom_Right_Layout->setContentsMargins(0, 0, 0, 0);
    Bottom_Right_Layout->setSpacing(8);

    QLabel *Chat_Title = new QLabel("Lobby Chat", Bottom_Right_Panel);
    Chat_Title->setStyleSheet(
        "font-size: 22px;"
        "font-weight: 700;"
        "color: rgb(28, 36, 58);");

    Chat_Display = new QTextEdit(Bottom_Right_Panel);
    Chat_Display->setReadOnly(true);
    Chat_Display->setPlainText(
        "The Void: The cosmos speaks through you.\n"
        "The Void:: Chat will appear here.");

    Chat_Input = new QLineEdit(Bottom_Right_Panel);
    Chat_Input->setPlaceholderText("Send a message into the cosmos...");

    Send_Chat_Button = new QPushButton("Send", Bottom_Right_Panel);
    Send_Chat_Button->setStyleSheet(Lobby_Button_Style);
    Send_Chat_Button->setMinimumWidth(100);

    QHBoxLayout *Chat_Input_Row = new QHBoxLayout();
    Chat_Input_Row->setContentsMargins(0, 0, 0, 0);
    Chat_Input_Row->setSpacing(8);
    Chat_Input_Row->addWidget(Chat_Input);
    Chat_Input_Row->addWidget(Send_Chat_Button);

    Bottom_Right_Layout->addWidget(Chat_Title);
    Bottom_Right_Layout->addWidget(Chat_Display, 1);
    Bottom_Right_Layout->addLayout(Chat_Input_Row);

    // timer for dots
    Ellipsis_Timer = new QTimer(this);

    connect(Ellipsis_Timer, &QTimer::timeout, this, [this]() {
        Update_Player_Status_Ellipses();
    });

    Ellipsis_Timer->start(500);
}

QLabel *Game_Multiplayer_Lobby_Elements::Create_Player_Row(const QString &playerLineText)
{
    QLabel *Player_Row_Label = new QLabel(playerLineText, Lobby_Widget);

    // keep rows compact
    Player_Row_Label->setWordWrap(false);
    Player_Row_Label->setMinimumHeight(22);
    Player_Row_Label->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);

    Player_Row_Label->setStyleSheet(
        "font-size: 10px;"
        "font-weight: 600;"
        "color: rgb(28, 36, 58);"
        "background-color: rgba(255,255,255,135);"
        "border: none;"
        "border-radius: 8px;"
        "padding: 3px 8px;");

    // keep base text so dots can animate
    Player_Row_Labels.append(Player_Row_Label);
    Player_Row_Base_Texts.append(playerLineText);

    return Player_Row_Label;
}

void Game_Multiplayer_Lobby_Elements::Update_Player_Status_Ellipses()
{
    // cycle 1 to 3 dots
    Ellipsis_Frame++;
    if(Ellipsis_Frame > 3)
        Ellipsis_Frame = 1;

    QString dots;
    for(int i = 0; i < Ellipsis_Frame; i++)
        dots += ".";

    for(int i = 0; i < Player_Row_Labels.size() && i < Player_Row_Base_Texts.size(); i++)
        Player_Row_Labels[i]->setText(Player_Row_Base_Texts[i] + dots);
}

void Game_Multiplayer_Lobby_Elements::Attach_Shared_Elements(Shared_Elements *sharedElements)
{
    // keep shared ref
    Shared_Group = sharedElements;

    Refresh_From_Shared_Elements();
}

void Game_Multiplayer_Lobby_Elements::Refresh_From_Shared_Elements()
{
    if(Shared_Group == nullptr)
        return;

    // main labels
    Lobby_Name_Label->setText(Shared_Group->Get_Lobby_Name());
    Connection_Status_Label->setText("Connection Status: " + Shared_Group->Get_Connection_Status());

    // only host sees host buttons
    const bool Is_Host = (Shared_Group->Get_Player_Role() == "Host");

    Top_Right_Panel->setVisible(Is_Host);
    Host_Options_Title_Label->setVisible(Is_Host);
    Start_Game_Button->setVisible(Is_Host);
    Edit_Lobby_Settings_Button->setVisible(Is_Host);
    Kick_Player_Button->setVisible(Is_Host);

    // clear old player rows
    while(Player_List_Layout->count() > 0)
    {
        QLayoutItem *Item = Player_List_Layout->takeAt(0);

        if(Item->widget() != nullptr)
            delete Item->widget();

        delete Item;
    }

    Player_Row_Labels.clear();
    Player_Row_Base_Texts.clear();

    // rebuild from shared player list
    QList<Player_Info> Players = Shared_Group->Get_Player_List();

    if(Players.isEmpty())
    {
        Player_List_Layout->addWidget(Create_Player_Row("No players in lobby"));
    }
    else
    {
        for(int i = 0; i < Players.size(); i++)
        {
            QString Role_Text = Players[i].Is_Host ? "Host" : "Player";
            QString Ready_Text = Players[i].Is_Ready ? "Ready" : "Not Ready";

            QString Row_Text = Players[i].Player_Name + " | " + Role_Text + " | " + Ready_Text;
            Player_List_Layout->addWidget(Create_Player_Row(Row_Text));
        }
    }

    Player_List_Layout->addStretch();
}

QWidget *Game_Multiplayer_Lobby_Elements::Get_Lobby_Widget() const
{
    return Lobby_Widget;
}
