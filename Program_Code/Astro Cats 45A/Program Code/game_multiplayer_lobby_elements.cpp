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
    //changed

    Lobby_Widget->setObjectName("Lobby_Widget");
    Lobby_Widget->setAttribute(Qt::WA_StyledBackground, true);

    Lobby_Widget->setStyleSheet(
        //widget
        "#Lobby_Widget {"
        "border-image: url(:/Images/Background/Space_BG_5.png) 0 0 0 0 stretch stretch;"
        //"color: rgb(28, 36, 58);"
        //"font-weight: bold;"
        //"font-family: 'OCR A Extended';"
        "}"
        //labels
        "QLabel {"
        "background: transparent;"
        "border: none;"
        //"color: rgb(28, 36, 58);"
        "color: rgb(245,245,250);"
        "font-family: 'OCR A Extended';"
        "}"
        //buttons
        "QPushButton {"
        "color: rgb(245,245,250);"
        "font-weight: bold;"
        "font-family: 'OCR A Extended';"
        "}"
        //chat
        "QTextEdit, QLineEdit {"
        "background-color: rgba(255, 255, 255, 185);"
        "color: rgb(245,245,250);"
        //"color: rgb(28, 36, 58);"
        "border: 1px solid rgba(120,150,210,170);"
        "border-radius: 12px;"
        "padding: 8px;"
        "font-family: 'OCR A Extended';"
        "}"
        //scroll bar
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
        "font-family: 'OCR A Extended';"
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
    Bottom_Middle_Panel = new QWidget(Lobby_Widget);
    Middle_Left_Panel = new QWidget(Lobby_Widget);
    Middle_Right_Panel = new QWidget(Lobby_Widget);
    Middle_Middle_Panel = new QWidget(Lobby_Widget);

    Top_Left_Panel->setStyleSheet("background: transparent; border: none;");
    Top_Middle_Panel->setStyleSheet("background: transparent; border: none;");
    Top_Right_Panel->setStyleSheet("background: transparent; border: none;");
    Bottom_Left_Panel->setStyleSheet("background: transparent; border: none;");
    Bottom_Right_Panel->setStyleSheet("background: transparent; border: none;");
    Bottom_Middle_Panel->setStyleSheet("background: transparent; border: none;");
    Middle_Left_Panel->setStyleSheet("background: transparent; border: none;");
    Middle_Right_Panel->setStyleSheet("background: transparent; border: none;");
    Middle_Middle_Panel->setStyleSheet("background: transparent; border: none;");

    Main_Layout->addWidget(Top_Left_Panel,    0, 0);
    Main_Layout->addWidget(Top_Middle_Panel,  0, 1);
    Main_Layout->addWidget(Top_Right_Panel,   0, 2);

    Main_Layout->addWidget(Middle_Left_Panel,   1, 0);
    Main_Layout->addWidget(Middle_Middle_Panel, 1, 1);
    Main_Layout->addWidget(Middle_Right_Panel,  1, 2);

    Main_Layout->addWidget(Bottom_Left_Panel,   2, 0);
    Main_Layout->addWidget(Bottom_Middle_Panel, 2, 1);
    Main_Layout->addWidget(Bottom_Right_Panel,  2, 2);

    Main_Layout->setColumnStretch(0, 2); // left narrower
    Main_Layout->setColumnStretch(1, 5); // middle bigger
    Main_Layout->setColumnStretch(2, 2); // chat narrower

    // =========================
    // top left : player list
    // =========================
    QVBoxLayout *Top_Left_Layout = new QVBoxLayout(Top_Left_Panel);
    Top_Left_Layout->setContentsMargins(0, 0, 0, 0);
    Top_Left_Layout->setSpacing(8);

    QLabel *Player_List_Title = new QLabel("Players", Top_Left_Panel);
    Player_List_Title->setStyleSheet(
        "font-size: 32px;"
        "font-weight: 700;"
        "color: rgb(245,245,250);"
    );

    Player_List_Scroll_Area = new QScrollArea(Top_Left_Panel);
    Player_List_Scroll_Area->setWidgetResizable(true);
    Player_List_Scroll_Area->setFrameShape(QFrame::NoFrame);
    Player_List_Scroll_Area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    Player_List_Scroll_Area->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    Player_List_Scroll_Area->setStyleSheet(
        "QScrollArea {"
        "background: transparent;"
        "border: none;"
        "}"
        "QScrollArea > QWidget > QWidget {"
        "background: transparent;"
        "}"
    );

    Player_List_Container = new QWidget();
    Player_List_Container->setStyleSheet("background: transparent; border: none;");

    Player_List_Layout = new QVBoxLayout(Player_List_Container);
    Player_List_Layout->setContentsMargins(0, 0, 0, 0);
    Player_List_Layout->setSpacing(8);

    Player_List_Layout->addStretch();

    Player_List_Scroll_Area->setWidget(Player_List_Container);

    Top_Left_Layout->addWidget(Player_List_Title);
    Top_Left_Layout->addWidget(Player_List_Scroll_Area);


    // =========================
    // top middle : lobby info
    // =========================
    QVBoxLayout *Top_Middle_Layout = new QVBoxLayout(Top_Middle_Panel);
    Top_Middle_Layout->setContentsMargins(0, 0, 0, 0);
    Top_Middle_Layout->setSpacing(12);

    Lobby_Name_Label = new QLabel("The Last Constellation Lobby", Top_Middle_Panel);
    Connection_Status_Label = new QLabel("Connection Status: Idle", Top_Middle_Panel);

    Lobby_Name_Label->setAlignment(Qt::AlignCenter);
    Connection_Status_Label->setAlignment(Qt::AlignCenter);

    Lobby_Name_Label->setWordWrap(true);
    Connection_Status_Label->setWordWrap(true);

    Lobby_Name_Label->setStyleSheet(
        "font-size: 34px;"
        "font-weight: 700;"
        "color: rgb(245,245,250);"
        "background-color: rgba(18, 32, 60, 170);"
        "border: 2px solid rgba(110,145,210,180);"
        "border-radius: 18px;"
        "padding: 10px 16px;"
    );

    Connection_Status_Label->setStyleSheet(
        "font-size: 22px;"
        "font-weight: 600;"
        "color: rgb(245,245,250);"
        "background-color: rgba(18, 32, 60, 150);"
        "border: 1px solid rgba(110,145,210,160);"
        "border-radius: 12px;"
        "padding: 8px 12px;"
    );

    Top_Middle_Layout->addStretch();
    Top_Middle_Layout->addWidget(Lobby_Name_Label);
    Top_Middle_Layout->addWidget(Connection_Status_Label);
    Top_Middle_Layout->addStretch();

    // =========================
    // top right : chat
    // =========================
    QVBoxLayout *Top_Right_Layout = new QVBoxLayout(Top_Right_Panel);
    Top_Right_Layout->setContentsMargins(0, 0, 0, 0);
    Top_Right_Layout->setSpacing(8);

    QLabel *Chat_Title = new QLabel("Lobby Chat", Top_Right_Panel);
    Chat_Title->setStyleSheet(
        "font-size: 30px;"
        "font-weight: 700;"
        "color: rgb(245,245,250);"
    );

    Chat_Display = new QTextEdit(Top_Right_Panel);
    Chat_Display->setReadOnly(true);
    Chat_Display->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    Chat_Display->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    Chat_Display->setMinimumHeight(260);
    Chat_Display->setPlainText(
        "The Void: The cosmos speaks through you.\n"
        "The Void: Chat will appear here."
    );
    Chat_Display->setStyleSheet(
        "font-size: 16px;"
        "background-color: rgba(18, 32, 60, 150);"
        "color: rgb(245,245,250);"
        "border: 1px solid rgba(120,150,210,170);"
        "border-radius: 12px;"
        "padding: 8px;"
    );

    Chat_Input = new QLineEdit(Top_Right_Panel);
    Chat_Input->setPlaceholderText("Send a message...");
    Chat_Input->setStyleSheet(
        "font-size: 16px;"
        "background-color: rgba(18, 32, 60, 180);"
        "color: rgb(245,245,250);"
        "border: 1px solid rgba(120,150,210,170);"
        "border-radius: 12px;"
        "padding: 8px;"
    );

    Send_Chat_Button = new QPushButton("Send", Top_Right_Panel);
    Send_Chat_Button->setStyleSheet(Lobby_Button_Style);
    Send_Chat_Button->setMinimumWidth(90);

    QHBoxLayout *Chat_Input_Row = new QHBoxLayout();
    Chat_Input_Row->setContentsMargins(0, 0, 0, 0);
    Chat_Input_Row->setSpacing(8);
    Chat_Input_Row->addWidget(Chat_Input);
    Chat_Input_Row->addWidget(Send_Chat_Button);

    Top_Right_Layout->addWidget(Chat_Title);
    Top_Right_Layout->addWidget(Chat_Display, 3);
    Top_Right_Layout->addLayout(Chat_Input_Row);

    // =========================
    // bottom left : host controls
    // =========================
    QVBoxLayout *Bottom_Left_Layout = new QVBoxLayout(Bottom_Left_Panel);
    Bottom_Left_Layout->setContentsMargins(0, 0, 0, 0);
    Bottom_Left_Layout->setSpacing(10);

    Host_Options_Title_Label = new QLabel("Host Controls", Bottom_Left_Panel);
    Host_Options_Title_Label->setStyleSheet(
        "font-size: 26px;"
        "font-weight: 700;"
        "color: rgb(245,245,250);"
    );

    Start_Game_Button = new QPushButton("Start Game", Bottom_Left_Panel);
    Edit_Lobby_Settings_Button = new QPushButton("Edit Lobby Settings", Bottom_Left_Panel);
    Kick_Player_Button = new QPushButton("Kick Player", Bottom_Left_Panel);

    Start_Game_Button->setStyleSheet(Lobby_Button_Style);
    Edit_Lobby_Settings_Button->setStyleSheet(Lobby_Button_Style);
    Kick_Player_Button->setStyleSheet(Lobby_Button_Style);

    Start_Game_Button->setMaximumWidth(260);
    Edit_Lobby_Settings_Button->setMaximumWidth(260);
    Kick_Player_Button->setMaximumWidth(260);

    Bottom_Left_Layout->addWidget(Host_Options_Title_Label, 0, Qt::AlignLeft);
    Bottom_Left_Layout->addWidget(Start_Game_Button, 0, Qt::AlignLeft);
    Bottom_Left_Layout->addWidget(Edit_Lobby_Settings_Button, 0, Qt::AlignLeft);
    Bottom_Left_Layout->addWidget(Kick_Player_Button, 0, Qt::AlignLeft);

    Bottom_Left_Layout->addStretch();

    // =========================
    // bottom middle : buttons
    // =========================
    QHBoxLayout *Bottom_Middle_Layout = new QHBoxLayout(Bottom_Middle_Panel);
    Bottom_Middle_Layout->setContentsMargins(0, 0, 0, 0);
    Bottom_Middle_Layout->setSpacing(12);

    Leave_Lobby_Button = new QPushButton("Leave Lobby", Bottom_Middle_Panel);
    Customize_Character_Button = new QPushButton("Customize Character", Bottom_Middle_Panel);
    Vote_Map_Button = new QPushButton("Vote Map", Bottom_Middle_Panel);

    Leave_Lobby_Button->setStyleSheet(Lobby_Button_Style);
    Customize_Character_Button->setStyleSheet(Lobby_Button_Style);
    Vote_Map_Button->setStyleSheet(Lobby_Button_Style);

    Leave_Lobby_Button->setMinimumWidth(170);
    Customize_Character_Button->setMinimumWidth(240);
    Vote_Map_Button->setMinimumWidth(170);

    Bottom_Middle_Layout->addStretch();
    Bottom_Middle_Layout->addWidget(Leave_Lobby_Button);
    Bottom_Middle_Layout->addWidget(Customize_Character_Button);
    Bottom_Middle_Layout->addWidget(Vote_Map_Button);
    Bottom_Middle_Layout->addStretch();

    // =========================
    // middle row : reserved for character display *************
    // =========================
    QVBoxLayout *Middle_Middle_Layout = new QVBoxLayout(Middle_Middle_Panel);
    Middle_Middle_Layout->setContentsMargins(0, 0, 0, 0);
    Middle_Middle_Layout->setSpacing(8);

    // leave empty for now
    Middle_Middle_Layout->addStretch();



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
