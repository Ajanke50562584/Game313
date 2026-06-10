#include "game_multiplayer_lobby_elements.h"
#include "shared_elements.h" // Game state
#include "game_networking_elements.h" // networking abilities

#include <QEvent> // To use eventfilter for resizing
#include <QColor>
#include <QFrame> // To remove border around player scroll area
#include <QFont> // Used to create fonts that can resize with window
#include <QGridLayout> // Main grid
#include <QHBoxLayout> // colomns in chat for text box and send button

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QCheckBox>
#include <QSpinBox>

#include <QSizePolicy> // Used for dynamic sizing

#include <QTextEdit>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
#include <QString>

#include <QTextCursor>// auto scrolling chat as new msgs appear

Game_Multiplayer_Lobby_Elements::Game_Multiplayer_Lobby_Elements(QWidget *parent)
    : QObject(parent)
{
    Networking_Group = nullptr; // dont point to anything, attach networking later with func
    Lobby_Widget = new QWidget(parent); // make visible UI
    Shared_Group = nullptr; // dynamicly attatch later

    Ellipsis_Timer = nullptr; // initialize the empty timer
    Ellipsis_Frame = 0;
    Easy_Votes = 0;
    Medium_Votes = 0;
    Hard_Votes = 0;
    Player_Circle_Holder = nullptr;

    Lobby_Widget->setAttribute(Qt::WA_StyledBackground, true); // allows style sheet background painting
    Lobby_Widget->installEventFilter(this); // Open widget's ears to resize events
    Lobby_Widget->setObjectName("Lobby_Widget");
    Lobby_Widget->setStyleSheet(
        "#Lobby_Widget "
        "{"
        "border-image: url(:/Images/Background/Space_BG_5.png) 0 0 0 0 stretch stretch;" // Dont crop image = slice 0 0 0 0, stretch hors stretch vert to fill widget
        "}"

        "QLabel "
        "{"
        "background: transparent;"
        "border: none;"
        "color: rgb(245,245,250);" // Text color
        "font-family: 'OCR A Extended';"
        "}"

        "QPushButton"
        "{"
        "color: rgb(245,245,250);" // button text
        "font-weight: bold;"
        "font-family: 'OCR A Extended';"
        "}"

        "QTextEdit, QLineEdit"
        "{"
        "background-color: rgba(255,255,255,185);"
        "color: rgb(245,245,250);"
        "border: 1px solid rgba(120,150,210,170);" // values for all RGBA found on online color picker
        "border-radius: 12px;"
        "padding: 8px;"
        "font-family: 'OCR A Extended';"
        "}"

        "QScrollArea "
        "{"
        "background: transparent;"
        "border: none;"
        "}"
    );

    // main grid
    QGridLayout *Main_Layout = new QGridLayout(Lobby_Widget);
    Main_Layout->setContentsMargins(24, 24, 24, 24);
    Main_Layout->setHorizontalSpacing(20); // between colomns
    Main_Layout->setVerticalSpacing(20); // between rows

    Top_Left_Panel = new QWidget(Lobby_Widget); // player list
    Top_Middle_Panel = new QWidget(Lobby_Widget); // Loby name / ready button
    Top_Right_Panel = new QWidget(Lobby_Widget); // chat
    Bottom_Left_Panel = new QWidget(Lobby_Widget); // host only
    Bottom_Middle_Panel = new QWidget(Lobby_Widget); // general loby buttons
    Middle_Middle_Panel = new QWidget(Lobby_Widget); // character display incoming

    Top_Left_Panel->setStyleSheet("background: transparent; border: none;");
    Top_Middle_Panel->setStyleSheet("background: transparent; border: none;");
    Top_Right_Panel->setStyleSheet("background: transparent; border: none;");
    Bottom_Left_Panel->setStyleSheet("background: transparent; border: none;");
    Bottom_Middle_Panel->setStyleSheet("background: transparent; border: none;");
    Middle_Middle_Panel->setStyleSheet("background: transparent; border: none;");

    Main_Layout->addWidget(Top_Left_Panel,0, 0); // row then colom
    Main_Layout->addWidget(Top_Middle_Panel,0, 1);
    Main_Layout->addWidget(Top_Right_Panel,0, 2);
    Main_Layout->addWidget(Middle_Middle_Panel, 1, 1);
    Main_Layout->addWidget(Bottom_Left_Panel,2, 0);
    Main_Layout->addWidget(Bottom_Middle_Panel,2, 1);

    // Which colomn then j=how much of fraction of availible width it gets
    Main_Layout->setColumnStretch(0, 2);
    Main_Layout->setColumnStretch(1, 5);
    Main_Layout->setColumnStretch(2, 2);
    Main_Layout->setRowStretch(0, 5);
    Main_Layout->setRowStretch(1, 2);
    Main_Layout->setRowStretch(2, 1);

    // top left player list
    QVBoxLayout *Top_Left_Layout = new QVBoxLayout(Top_Left_Panel); // Layout for player list
    Top_Left_Layout->setContentsMargins(0, 0, 0, 0); // no extra space
    Top_Left_Layout->setSpacing(8); // 8 px between rows

    QLabel *Player_List_Title = new QLabel("Players", Top_Left_Panel); // Heading label
    Player_List_Title->setStyleSheet("font-size: 32px; font-weight: 700; color: rgb(245,245,250);");

    Player_List_Scroll_Area = new QScrollArea(Top_Left_Panel);
    Player_List_Scroll_Area->setWidgetResizable(true);
    Player_List_Scroll_Area->setFrameShape(QFrame::NoFrame); // remove defualt frame from area
    Player_List_Scroll_Area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    Player_List_Scroll_Area->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    Player_List_Scroll_Area->setStyleSheet(
                "QScrollArea"
                "{"
                "background: transparent;"
                "border: none;"
                "}"
                "QScrollArea > QWidget > QWidget" // the widget in the widget in the scroll area
                "{"
                "background: transparent;"
                "}"
                );

    Player_List_Container = new QWidget();//make widget containing the rows
    Player_List_Container->setStyleSheet("background: transparent; border: none;"); // only show content of container

    Player_List_Layout = new QVBoxLayout(Player_List_Container); // Vert layout in box
    Player_List_Layout->setContentsMargins(0, 0, 0, 0); // no inner spacing
    Player_List_Layout->setSpacing(8);//8 px between player rows

    Player_List_Layout->addStretch(); // add padding so player rows tay pushed towards the top

    Player_List_Scroll_Area->setWidget(Player_List_Container); // Put player list in container

    Top_Left_Layout->addWidget(Player_List_Title); // add title to top left panel
    Top_Left_Layout->addWidget(Player_List_Scroll_Area); // add scroll area in top left aswell



    QVBoxLayout *Top_Middle_Layout = new QVBoxLayout(Top_Middle_Panel); // Vert layout for Loby name and ready
    Top_Middle_Layout->setContentsMargins(0, 0, 0, 0); // No margin inside
    Top_Middle_Layout->setSpacing(12); // 12 px between rows

    Lobby_Name_Label = new QLabel("The Last Constellation Lobby", Top_Middle_Panel); // Loby name label
    Ready_Button = new QPushButton("Not Ready", Top_Middle_Panel); // Button togling ready state

    Lobby_Name_Label->setAlignment(Qt::AlignCenter);
    Lobby_Name_Label->setWordWrap(true);
    Lobby_Name_Label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    Lobby_Name_Label->setStyleSheet(
                "font-size: 34px;"
                "font-weight: 600;" // semi bold
                "color: rgb(245,245,250);"
                "background-color: rgba(18, 32, 60, 170);"
                "border: 2px solid rgba(110,145,210,180);"
                "border-radius: 18px;"
                "padding: 10px 16px;"
                );

    connect(Ready_Button, SIGNAL(clicked()), this, SLOT(Handle_Ready_Toggled())); //When Clicked toggle ready state

    Top_Middle_Layout->addStretch(1); // Padding above loby name
    Top_Middle_Layout->addWidget(Lobby_Name_Label); // add loby name label to top middel panel
    Top_Middle_Layout->addWidget(Ready_Button, 0, Qt::AlignHCenter); // add ready button to panel
    Top_Middle_Layout->addStretch(1); // stretch to panel

    QVBoxLayout *Top_Right_Layout = new QVBoxLayout(Top_Right_Panel); // Chat vert layour
    Top_Right_Layout->setContentsMargins(0, 0, 0, 0);
    Top_Right_Layout->setSpacing(8);// 8 px between chat title, display and then input row

    QLabel *Chat_Title = new QLabel("Cosmos Chat", Top_Right_Panel); // Chat title
    Chat_Title->setStyleSheet(
                "font-size: 30px;"
                "font-weight: 600;"
                "color: rgb(245,245,250);"
                );

    Chat_Display = new QTextEdit(Top_Right_Panel); // Chat history
    Chat_Display->setReadOnly(true);
    Chat_Display->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    Chat_Display->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    Chat_Display->setLineWrapMode(QTextEdit::WidgetWidth);
    Chat_Display->setMinimumHeight(120);
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

    Chat_Input = new QLineEdit(Top_Right_Panel); // chat input
    Chat_Input->setPlaceholderText("Send a message...");
    Chat_Input->setStyleSheet(
                "font-size: 6px;"
                "background-color: rgba(18, 32, 60, 180);"
                "color: rgb(245,245,250);"
                "border: 1px solid rgba(120,150,210,170);"
                "border-radius: 12px;"
                "padding: 1px;"
                );

    Send_Chat_Button = new QPushButton("Send", Top_Right_Panel);// ssend button
    Send_Chat_Button->setMinimumWidth(96);
    Send_Chat_Button->setMaximumWidth(120);
    Send_Chat_Button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    connect(Send_Chat_Button, SIGNAL(clicked()), this, SLOT(Send_To_Chat()));// clicking and entering sends to network
    connect(Chat_Input, SIGNAL(returnPressed()), this, SLOT(Send_To_Chat()));

    QHBoxLayout *Chat_Input_Row = new QHBoxLayout(); // the input line edit and send button colomns
    Chat_Input_Row->setContentsMargins(0, 0, 0, 0);
    Chat_Input_Row->setSpacing(8);
    Chat_Input_Row->addWidget(Chat_Input,1);
    Chat_Input_Row->addWidget(Send_Chat_Button,0);


    Top_Right_Layout->addWidget(Chat_Title);
    Top_Right_Layout->addWidget(Chat_Display, 3); // Gets extra space compared to title and input
    Top_Right_Layout->addLayout(Chat_Input_Row);


    QVBoxLayout *Bottom_Left_Layout = new QVBoxLayout(Bottom_Left_Panel);// host controll layout
    Bottom_Left_Layout->setContentsMargins(0, 0, 0, 0);
    Bottom_Left_Layout->setSpacing(10);

    Host_Options_Title_Label = new QLabel("Host Controls", Bottom_Left_Panel);// titel label
    Host_Options_Title_Label->setStyleSheet("font-size: 26px; font-weight: 700; color: rgb(245,245,250);");

    Start_Game_Button = new QPushButton("Start Game", Bottom_Left_Panel);
    Edit_Lobby_Settings_Button = new QPushButton("Edit Lobby\nSettings", Bottom_Left_Panel);
    Kick_Player_Button = new QPushButton("Kick Player", Bottom_Left_Panel);

    Start_Game_Button->setMaximumWidth(260);
    Edit_Lobby_Settings_Button->setMaximumWidth(260);
    Kick_Player_Button->setMaximumWidth(260);

    Bottom_Left_Layout->addWidget(Host_Options_Title_Label, 0, Qt::AlignLeft); // add buttons to the layout
    Bottom_Left_Layout->addWidget(Start_Game_Button, 0, Qt::AlignLeft);
    Bottom_Left_Layout->addWidget(Edit_Lobby_Settings_Button, 0, Qt::AlignLeft);
    Bottom_Left_Layout->addWidget(Kick_Player_Button, 0, Qt::AlignLeft);

    Bottom_Left_Layout->addStretch();

    // bottom middle buttons
    QHBoxLayout *Bottom_Middle_Layout = new QHBoxLayout(Bottom_Middle_Panel);
    Bottom_Middle_Layout->setContentsMargins(0, 0, 0, 0);
    Bottom_Middle_Layout->setSpacing(12);

    Leave_Lobby_Button = new QPushButton("Leave Lobby", Bottom_Middle_Panel);
    Customize_Character_Button = new QPushButton("Customize\nCharacter", Bottom_Middle_Panel);
    Vote_Map_Button = new QPushButton("Vote Map", Bottom_Middle_Panel);

    // Connects for all buttons
    connect(Leave_Lobby_Button, SIGNAL(clicked()), this, SLOT(Handle_Leave_Lobby()));
    connect(Customize_Character_Button, SIGNAL(clicked()), this, SLOT(Handle_Customize_Character()));
    connect(Vote_Map_Button, SIGNAL(clicked()), this, SLOT(Handle_Vote_Map()));
    connect(Start_Game_Button, SIGNAL(clicked()), this, SLOT(Handle_Start_Game()));
    connect(Edit_Lobby_Settings_Button, SIGNAL(clicked()), this, SLOT(Handle_Edit_Lobby_Settings()));
    connect(Kick_Player_Button, SIGNAL(clicked()), this, SLOT(Handle_Kick_Player()));

    Leave_Lobby_Button->setMinimumWidth(170);
    Customize_Character_Button->setMinimumWidth(240);
    Vote_Map_Button->setMinimumWidth(170);

    //Add buttons to bottom mid layout
    Bottom_Middle_Layout->addStretch();
    Bottom_Middle_Layout->addWidget(Leave_Lobby_Button);
    Bottom_Middle_Layout->addWidget(Customize_Character_Button);
    Bottom_Middle_Layout->addWidget(Vote_Map_Button);
    Bottom_Middle_Layout->addStretch();

    // middle row reserved for character display
    QVBoxLayout *Middle_Middle_Layout = new QVBoxLayout(Middle_Middle_Panel);
    Middle_Middle_Layout->setContentsMargins(0, 0, 0, 0);
    Middle_Middle_Layout->setSpacing(8);

    Difficulty_Popup = new QWidget(Middle_Middle_Panel);
    Difficulty_Popup->setVisible(false);

    QVBoxLayout *Difficulty_Layout = new QVBoxLayout(Difficulty_Popup);
    Difficulty_Layout->setContentsMargins(14, 14, 14, 14);
    Difficulty_Layout->setSpacing(8);

    Difficulty_Title_Label = new QLabel("Map Difficulty", Difficulty_Popup);
    Difficulty_Title_Label->setAlignment(Qt::AlignCenter);

    Difficulty_Current_Label = new QLabel("Selected: Medium", Difficulty_Popup);
    Difficulty_Current_Label->setAlignment(Qt::AlignCenter);

    Easy_Difficulty_Button = new QPushButton("Easy", Difficulty_Popup);
    Medium_Difficulty_Button = new QPushButton("Medium", Difficulty_Popup);
    Hard_Difficulty_Button = new QPushButton("Hard", Difficulty_Popup);

    Easy_Map_Preview_Label = new QLabel("", Difficulty_Popup);
    Medium_Map_Preview_Label = new QLabel("", Difficulty_Popup);
    Hard_Map_Preview_Label = new QLabel("", Difficulty_Popup);
    Easy_Map_Preview_Label->setMinimumSize(90, 54);
    Medium_Map_Preview_Label->setMinimumSize(90, 54);
    Hard_Map_Preview_Label->setMinimumSize(90, 54);
    Easy_Map_Preview_Label->setStyleSheet("background-color: rgba(80,160,110,190); border: 2px solid rgba(235,245,255,180);");
    Medium_Map_Preview_Label->setStyleSheet("background-color: rgba(190,160,70,190); border: 2px solid rgba(235,245,255,180);");
    Hard_Map_Preview_Label->setStyleSheet("background-color: rgba(185,70,90,190); border: 2px solid rgba(235,245,255,180);");

    QHBoxLayout *Difficulty_Button_Row = new QHBoxLayout();
    Difficulty_Button_Row->setSpacing(8);
    Difficulty_Button_Row->addWidget(Easy_Difficulty_Button);
    Difficulty_Button_Row->addWidget(Medium_Difficulty_Button);
    Difficulty_Button_Row->addWidget(Hard_Difficulty_Button);

    QHBoxLayout *Difficulty_Preview_Row = new QHBoxLayout();
    Difficulty_Preview_Row->setSpacing(8);
    Difficulty_Preview_Row->addWidget(Easy_Map_Preview_Label);
    Difficulty_Preview_Row->addWidget(Medium_Map_Preview_Label);
    Difficulty_Preview_Row->addWidget(Hard_Map_Preview_Label);

    Difficulty_Layout->addWidget(Difficulty_Title_Label);
    Difficulty_Layout->addWidget(Difficulty_Current_Label);
    Difficulty_Layout->addLayout(Difficulty_Preview_Row);
    Difficulty_Layout->addLayout(Difficulty_Button_Row);

    connect(Easy_Difficulty_Button, SIGNAL(clicked()), this, SLOT(Handle_Easy_Difficulty()));
    connect(Medium_Difficulty_Button, SIGNAL(clicked()), this, SLOT(Handle_Medium_Difficulty()));
    connect(Hard_Difficulty_Button, SIGNAL(clicked()), this, SLOT(Handle_Hard_Difficulty()));

    Lobby_Settings_Popup = new QWidget(Middle_Middle_Panel);
    Lobby_Settings_Popup->setVisible(false);

    QVBoxLayout *Settings_Layout = new QVBoxLayout(Lobby_Settings_Popup);
    Settings_Layout->setContentsMargins(14, 14, 14, 14);
    Settings_Layout->setSpacing(8);

    QLabel *Settings_Title_Label = new QLabel("Lobby Settings", Lobby_Settings_Popup);
    Settings_Title_Label->setAlignment(Qt::AlignCenter);
    Lobby_Public_Checkbox = new QCheckBox("Public Lobby", Lobby_Settings_Popup);
    Lobby_Public_Checkbox->setChecked(true);
    Lobby_Password_Input = new QLineEdit(Lobby_Settings_Popup);
    Lobby_Password_Input->setPlaceholderText("Password");
    Lobby_Max_Players_Input = new QSpinBox(Lobby_Settings_Popup);
    Lobby_Max_Players_Input->setRange(2, 4);
    Lobby_Max_Players_Input->setValue(4);
    Apply_Lobby_Settings_Button = new QPushButton("Apply", Lobby_Settings_Popup);

    Settings_Layout->addWidget(Settings_Title_Label);
    Settings_Layout->addWidget(Lobby_Public_Checkbox);
    Settings_Layout->addWidget(Lobby_Password_Input);
    Settings_Layout->addWidget(Lobby_Max_Players_Input);
    Settings_Layout->addWidget(Apply_Lobby_Settings_Button);

    connect(Apply_Lobby_Settings_Button, SIGNAL(clicked()), this, SLOT(Handle_Apply_Lobby_Settings()));

    Middle_Middle_Layout->addStretch(0);
    Middle_Middle_Layout->addWidget(Difficulty_Popup, 0, Qt::AlignCenter);
    Middle_Middle_Layout->addWidget(Lobby_Settings_Popup, 0, Qt::AlignCenter);
    Middle_Middle_Layout->addStretch(0);

    // timer for dots
    Ellipsis_Timer = new QTimer(this);
    connect(Ellipsis_Timer, SIGNAL(timeout()), this, SLOT(Animate_Player_Status_Ellipses()));

    Ellipsis_Timer->start(750);
    Apply_Responsive_Sizing(); // Allows entire screen to resize dynamically
}

QLabel *Game_Multiplayer_Lobby_Elements::Create_Player_Row(const QString &playerLineText) // function to create each player from the networking data
{
    QLabel *Player_Row_Label = new QLabel(playerLineText, Lobby_Widget);

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

void Game_Multiplayer_Lobby_Elements::Update_Player_Status_Ellipses()// animates dots after each player name
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

void Game_Multiplayer_Lobby_Elements::Attach_Shared_Elements(Shared_Elements *sharedElements) // ataches shared loby state
{
    // keep shared ref
    Shared_Group = sharedElements;
    Refresh_From_Shared_Elements(); // refresh ui after new shared data added
}

void Game_Multiplayer_Lobby_Elements::Attach_Networking_Elements(Game_Networking_Elements *networkingElements)
{
    Networking_Group = networkingElements;
    qDebug() << "Lobby networking attached";
}

void Game_Multiplayer_Lobby_Elements::Add_Chat_Message(const QString &message)
{
    Chat_Display->append(message);
    Chat_Display->moveCursor(QTextCursor::End);// auto scroll to newestt messages
}

void Game_Multiplayer_Lobby_Elements::Refresh_From_Shared_Elements() // rebuilds loby from shared state
{
    if(Shared_Group == nullptr) // immediately stop if shared data hasnt been attached
        return;

    // main labels
    Lobby_Name_Label->setText(Shared_Group->Get_Lobby_Name()); // from host setup
    Ready_Button->setText(Get_Local_Player_Ready_State() ? "Ready" : "Not Ready"); // shows if player is ready or not
    Difficulty_Current_Label->setText(QString("Selected: %1  E:%2 M:%3 H:%4")
                                      .arg(Shared_Group->Get_Map_Difficulty())
                                      .arg(Shared_Group->Get_Easy_Votes())
                                      .arg(Shared_Group->Get_Medium_Votes())
                                      .arg(Shared_Group->Get_Hard_Votes()));
    Lobby_Public_Checkbox->setChecked(Shared_Group->Get_Is_Public_Lobby());
    Lobby_Max_Players_Input->setValue(Shared_Group->Get_Max_Players());
    Lobby_Password_Input->setText(Shared_Group->Get_Lobby_Password());

    // keep chat available to host and clients, but only show host tools to the host
    const bool Is_Host = (Shared_Group->Get_Player_Role() == "Host"); // variable storing wether playeer is host or not

    Top_Right_Panel->setVisible(true);
    Host_Options_Title_Label->setVisible(Is_Host);// only show if host
    Start_Game_Button->setVisible(Is_Host);// only show if host
    Edit_Lobby_Settings_Button->setVisible(Is_Host);// only show if host
    Kick_Player_Button->setVisible(Is_Host);// only show if host

    // clear old player rows
    while(Player_List_Layout->count() > 0)
    {
        QLayoutItem *Item = Player_List_Layout->takeAt(0);// take fist item out layout

        if(Item->widget() != nullptr)
            delete Item->widget(); // if layout contains  widget clear widget

        delete Item; // deletes layout item
    }

    // clear label pointers and base texts
    Player_Row_Labels.clear();
    Player_Row_Base_Texts.clear();

    // rebuild from shared player list
    QList<Player_Info> Players = Shared_Group->Get_Player_List();

    if(Players.isEmpty())
    {
        Player_List_Layout->addWidget(Create_Player_Row("No players in lobby")); // if shared space has no players
    }
    else
    {
        for(int i = 0; i < Players.size(); i++)
        {
            QString Role_Text = Players[i].Is_Host ? "Host" : "Player";
            QString Ready_Text = Players[i].Is_Ready ? "Ready" : "Not Ready";

            QString Row_Text = Players[i].Player_Name + " | " + Role_Text + " | " + Ready_Text; // builds rows
            Player_List_Layout->addWidget(Create_Player_Row(Row_Text)); // add row to layout
        }
    }

    Player_List_Layout->addStretch(); // keep players towards top
    Rebuild_Player_Circles();
}

QWidget *Game_Multiplayer_Lobby_Elements::Get_Lobby_Widget() const
{
    return Lobby_Widget;
}

bool Game_Multiplayer_Lobby_Elements::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == Lobby_Widget && event->type() == QEvent::Resize)
        Apply_Responsive_Sizing();

    return QObject::eventFilter(watched, event);
}

void Game_Multiplayer_Lobby_Elements::Apply_Responsive_Sizing()
{
    if(Lobby_Widget == nullptr)
        return; // if loby no exist no go here

    const int w = qMax(Lobby_Widget->width(), 640); // width never treated as less than 640
    const int h = qMax(Lobby_Widget->height(), 360); // height never treated as less than 360

    QGridLayout *Main_Layout = qobject_cast<QGridLayout*>(Lobby_Widget->layout());// gets and tries to cast loby layout to gridlaout
    if(Main_Layout != nullptr)// only continue if successfull cast
    {
        Main_Layout->setContentsMargins(qMax(12, (24 * w) / 1280), qMax(12, (24 * h) / 720), qMax(12, (24 * w) / 1280),qMax(12, (24 * h) / 720)); // set responsive margins but never go below 12 px
        Main_Layout->setHorizontalSpacing(qMax(10, (20 * w) / 1280)); // responsive never go below 10
        Main_Layout->setVerticalSpacing(qMax(10, (20 * h) / 720));
    }

    QFont Title_Font("OCR A Extended");
    Title_Font.setPointSize(qMax(12, w / 52)); // never smaller than 12
    Title_Font.setBold(true);
    Lobby_Name_Label->setFont(Title_Font);

    QFont Status_Font("OCR A Extended");
    Status_Font.setPointSize(qMax(9, w / 72)); // never smaller than 9
    Status_Font.setBold(true);
    Ready_Button->setFont(Status_Font);

    QFont Section_Font("OCR A Extended");
    Section_Font.setPointSize(qMax(10, w / 68));// never smaller than 10
    Section_Font.setBold(true);

    QFont Body_Font("OCR A Extended");
    Body_Font.setPointSize(qMax(8, w / 105)); // never smaller than 8

    QFont Chat_Font("OCR A Extended");
    Chat_Font.setPointSize(qMax(6, w / 100)); // never smaller than 8

    const QList<QLabel*> Section_Labels =
    {
        qobject_cast<QLabel*>(Top_Left_Panel->layout()->itemAt(0)->widget()), // Create temp list of heading lables
        qobject_cast<QLabel*>(Top_Right_Panel->layout()->itemAt(0)->widget()),
        Host_Options_Title_Label
    };

    for(int i = 0; i < Section_Labels.size(); i++) // apply font to said labels
    {
        if(Section_Labels[i] != nullptr)
            Section_Labels[i]->setFont(Section_Font);
    }

    // Sets fonts for all texts
    Chat_Display->setFont(Chat_Font);
    Chat_Input->setFont(Chat_Font);
    Send_Chat_Button->setFont(Body_Font);
    Start_Game_Button->setFont(Body_Font);
    Edit_Lobby_Settings_Button->setFont(Body_Font);
    Kick_Player_Button->setFont(Body_Font);
    Leave_Lobby_Button->setFont(Body_Font);
    Customize_Character_Button->setFont(Body_Font);
    Vote_Map_Button->setFont(Body_Font);
    Easy_Difficulty_Button->setFont(Body_Font);
    Medium_Difficulty_Button->setFont(Body_Font);
    Hard_Difficulty_Button->setFont(Body_Font);
    Difficulty_Title_Label->setFont(Section_Font);
    Difficulty_Current_Label->setFont(Body_Font);
    Lobby_Public_Checkbox->setFont(Body_Font);
    Lobby_Password_Input->setFont(Body_Font);
    Lobby_Max_Players_Input->setFont(Body_Font);
    Apply_Lobby_Settings_Button->setFont(Body_Font);

    const int Button_Height = qMax(30, (34 * h) / 720); // Sets button heights
    const int Multi_Line_Button_Height = qMax(40, (46 * h) / 720);
    const int Host_Button_Width = qMax(130, (180 * w) / 1280);
    const int Action_Button_Width = qMax(100, (135 * w) / 1280);
    const int Wide_Action_Button_Width = qMax(130, (160 * w) / 1280);
    const int Send_Button_Width = qMax(70, (108 * w) / 1280);
    const int Ready_Button_Width = qMax(120, (170 * w) / 1280);

    const int Chat_Min_Height = qMax(110, (180 * h) / 720); //Set chat height

    const int Player_Row_Height = qMax(20, (22 * h) / 720); //Set player row height

    Send_Chat_Button->setMinimumWidth(Send_Button_Width); //set button width
    Send_Chat_Button->setMaximumWidth(Send_Button_Width);
    Send_Chat_Button->setMinimumHeight(Button_Height);
    Send_Chat_Button->setMaximumHeight(Button_Height);

    Ready_Button->setMinimumWidth(Ready_Button_Width);
    Ready_Button->setMinimumHeight(Button_Height);
    Ready_Button->setMaximumHeight(Button_Height + 18);

    Chat_Input->setMinimumWidth(qMax(100, (260 * w) / 1280));

    Start_Game_Button->setMinimumWidth(Host_Button_Width);
    Start_Game_Button->setMaximumWidth(Host_Button_Width);
    Edit_Lobby_Settings_Button->setMinimumWidth(Host_Button_Width);
    Edit_Lobby_Settings_Button->setMaximumWidth(Host_Button_Width);
    Kick_Player_Button->setMinimumWidth(Host_Button_Width);
    Kick_Player_Button->setMaximumWidth(Host_Button_Width);
    Start_Game_Button->setMinimumHeight(Button_Height);
    Edit_Lobby_Settings_Button->setMinimumHeight(Multi_Line_Button_Height);
    Kick_Player_Button->setMinimumHeight(Button_Height);

    Leave_Lobby_Button->setMinimumWidth(Action_Button_Width);
    Customize_Character_Button->setMinimumWidth(Wide_Action_Button_Width);
    Vote_Map_Button->setMinimumWidth(Action_Button_Width);
    Leave_Lobby_Button->setMinimumHeight(Button_Height);
    Customize_Character_Button->setMinimumHeight(Multi_Line_Button_Height);
    Vote_Map_Button->setMinimumHeight(Button_Height);
    Leave_Lobby_Button->setMaximumHeight(Button_Height + 14);
    Customize_Character_Button->setMaximumHeight(Multi_Line_Button_Height + 12);
    Vote_Map_Button->setMaximumHeight(Button_Height + 14);
    Easy_Difficulty_Button->setMinimumHeight(Button_Height);
    Medium_Difficulty_Button->setMinimumHeight(Button_Height);
    Hard_Difficulty_Button->setMinimumHeight(Button_Height);
    Apply_Lobby_Settings_Button->setMinimumHeight(Button_Height);

    Leave_Lobby_Button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    Customize_Character_Button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    Vote_Map_Button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    Chat_Display->setMinimumHeight(Chat_Min_Height);
    Chat_Display->setMaximumHeight(qMax(190, (280 * h) / 720));

    for(int i = 0; i < Player_Row_Labels.size(); i++) // Set min height for all player rows
        Player_Row_Labels[i]->setMinimumHeight(Player_Row_Height);

    Apply_Responsive_Styles(); // Re-apply styles so padding and border radii also scale
}

void Game_Multiplayer_Lobby_Elements::Apply_Responsive_Styles()
{
    if(Lobby_Widget == nullptr)
        return;
    //min dimensions ensures nothing becomes to small
    const int w = qMax(Lobby_Widget->width(), 640);
    const int h = qMax(Lobby_Widget->height(), 360);

    // These values scale spacing, rounded corners, and padding with the window size.
    const int Panel_Spacing = qMax(6, (8 * h) / 720);
    const int Title_Radius = qMax(12, (18 * h) / 720);
    const int Small_Radius = qMax(8, (12 * h) / 720);
    const int Button_Radius = qMax(12, (18 * h) / 720);
    const int Button_Pad_V = qMax(6, (10 * h) / 720);
    const int Button_Pad_H = qMax(10, (18 * w) / 1280);
    const int Input_Pad = qMax(6, (8 * h) / 720);
    const int Player_Radius = qMax(6, (8 * h) / 720);
    const int Player_Pad_V = qMax(2, (3 * h) / 720);
    const int Player_Pad_H = qMax(6, (8 * w) / 1280);

    // Shared button stylesheet; %1, %2, and %3 are rspectively filled by .arg(...) below
    // %1 = radius, %2 = vertical padding, %3 = horizontal padding.
    const QString Lobby_Button_Style = QString(
                "QPushButton "
                "{"
                "background-color: rgba(18, 32, 60, 220);"
                "color: rgb(245,245,250);"
                "border: 2px solid rgba(110,145,210,210);"
                "border-radius: %1px;"
                "padding: %2px %3px;"
                "font-weight: 700;"
                "font-family: 'OCR A Extended';"
                "text-align: center;"
                "}"

                "QPushButton:hover"
                "{"
                "background-color: rgba(32, 52, 92, 235);"
                "}"

                "QPushButton:pressed"
                "{"
                "background-color: rgba(12, 22, 42, 245);"
                "}")

            .arg(Button_Radius) .arg(Button_Pad_V) .arg(Button_Pad_H); // becuase everything is dynamic I dont hard code it into the style sheet


    Send_Chat_Button->setStyleSheet(Lobby_Button_Style);
    Start_Game_Button->setStyleSheet(Lobby_Button_Style);
    Edit_Lobby_Settings_Button->setStyleSheet(Lobby_Button_Style);
    Kick_Player_Button->setStyleSheet(Lobby_Button_Style);
    Leave_Lobby_Button->setStyleSheet(Lobby_Button_Style);
    Customize_Character_Button->setStyleSheet(Lobby_Button_Style);
    Vote_Map_Button->setStyleSheet(Lobby_Button_Style);
    Easy_Difficulty_Button->setStyleSheet(Lobby_Button_Style);
    Medium_Difficulty_Button->setStyleSheet(Lobby_Button_Style);
    Hard_Difficulty_Button->setStyleSheet(Lobby_Button_Style);
    Apply_Lobby_Settings_Button->setStyleSheet(Lobby_Button_Style);

    Difficulty_Popup->setStyleSheet(QString(
                                        "background-color: rgba(18, 32, 60, 205);"
                                        "border: 2px solid rgba(110,145,210,190);"
                                        "border-radius: %1px;")
                                    .arg(Title_Radius));

    Lobby_Settings_Popup->setStyleSheet(QString(
                                            "background-color: rgba(18, 32, 60, 205);"
                                            "border: 2px solid rgba(110,145,210,190);"
                                            "border-radius: %1px;")
                                        .arg(Title_Radius));

    Lobby_Password_Input->setStyleSheet(QString(
                                            "background-color: rgba(18, 32, 60, 180);"
                                            "color: rgb(245,245,250);"
                                            "border: 1px solid rgba(120,150,210,170);"
                                            "border-radius: %1px;"
                                            "padding: %2px;")
                                        .arg(Small_Radius)
                                        .arg(Input_Pad));

    Lobby_Name_Label->setStyleSheet(QString(
                                        "color: rgb(245,245,250);"
                                        "background-color: rgba(18, 32, 60, 170);"
                                        "border: 2px solid rgba(110,145,210,180);"
                                        "border-radius: %1px;"
                                        "padding: %2px %3px;")
                                    .arg(Title_Radius) .arg(qMax(7, (10 * h) / 720)) .arg(qMax(10, (16 * w) / 1280)));

    // Ready button changes color depending on whether the local player is ready
    const bool Is_Ready = Get_Local_Player_Ready_State();

    // QString placeholders are filled by the .arg(...) calls in order:
    // %1 = normal background, %2 = border color, %3 = border radius,
    // %4 = vertical padding, %5 = horizontal padding,
    // %6 = hover background, %7 = pressed background.

    Ready_Button->setStyleSheet(QString(
                                    "QPushButton"
                                    "{"
                                    "color: rgb(245,245,250);"
                                    "background-color: %1;"
                                    "border: 2px solid %2;"
                                    "border-radius: %3px;"
                                    "padding: %4px %5px;"
                                    "font-weight: 700;"
                                    "font-family: 'OCR A Extended';"
                                    "}"

                                    "QPushButton:hover"
                                    "{"
                                    "background-color: %6;"
                                    "}"

                                    "QPushButton:pressed"
                                    "{"
                                    "background-color: %7;"
                                    "}")

                                .arg(Is_Ready ? "rgba(36, 116, 84, 220)" : "rgba(18, 32, 60, 170)")
                                .arg(Is_Ready ? "rgba(112,220,170,210)" : "rgba(110,145,210,180)")
                                .arg(Small_Radius)
                                .arg(qMax(5, (8 * h) / 720))
                                .arg(qMax(12, (18 * w) / 1280))
                                .arg(Is_Ready ? "rgba(52, 142, 102, 235)" : "rgba(32, 52, 92, 210)")
                                .arg(Is_Ready ? "rgba(24, 90, 64, 245)" : "rgba(12, 22, 42, 225)"));

    Chat_Display->setStyleSheet(QString(
                                    "background-color: rgba(18, 32, 60, 150);"
                                    "color: rgb(245,245,250);"
                                    "border: 1px solid rgba(120,150,210,170);"
                                    "border-radius: %1px;"
                                    "padding: %2px;")
                                .arg(Small_Radius)
                                .arg(Input_Pad));

    Chat_Input->setStyleSheet(QString(
                                  "background-color: rgba(18, 32, 60, 180);"
                                  "color: rgb(245,245,250);"
                                  "border: 1px solid rgba(120,150,210,170);"
                                  "border-radius: %1px;"
                                  "padding: %2px;")
                              .arg(Small_Radius)
                              .arg(Input_Pad));

    for(int i = 0; i < Player_Row_Labels.size(); i++) // apply styling to each player row
    {
        Player_Row_Labels[i]->setStyleSheet(QString(
                                                "font-weight: 600;"
                                                "color: rgb(28, 36, 58);"
                                                "background-color: rgba(255,255,255,135);"
                                                "border: none;"
                                                "border-radius: %1px;"
                                                "padding: %2px %3px;")
                                            .arg(Player_Radius)
                                            .arg(Player_Pad_V)
                                            .arg(Player_Pad_H));
    }

    // Update layout spacing only if the panel has a layout attached.
    // Some panels use shared Panel_Spacing, while others use custom responsive spacing.
    if(Top_Left_Panel->layout() != nullptr)
        Top_Left_Panel->layout()->setSpacing(Panel_Spacing);

    if(Top_Middle_Panel->layout() != nullptr)
        Top_Middle_Panel->layout()->setSpacing(qMax(8, (12 * h) / 720));

    if(Top_Right_Panel->layout() != nullptr)
        Top_Right_Panel->layout()->setSpacing(Panel_Spacing);

    if(Bottom_Left_Panel->layout() != nullptr)
        Bottom_Left_Panel->layout()->setSpacing(qMax(8, (10 * h) / 720));

    if(Bottom_Middle_Panel->layout() != nullptr)
        Bottom_Middle_Panel->layout()->setSpacing(qMax(8, (12 * h) / 720));

    if(Middle_Middle_Panel->layout() != nullptr)
        Middle_Middle_Panel->layout()->setSpacing(Panel_Spacing);
  }

void Game_Multiplayer_Lobby_Elements::Animate_Player_Status_Ellipses() // timer helper
{
    Update_Player_Status_Ellipses();
}

bool Game_Multiplayer_Lobby_Elements::Get_Local_Player_Ready_State() const //reads the local ready state from shared data.
{
    if(Shared_Group == nullptr)
        return false;

    return Shared_Group->Get_Local_Player_Ready();
}

void Game_Multiplayer_Lobby_Elements::Handle_Ready_Toggled() // Flips the local player's ready state and refreshes the lobby UI.
{
    if(Shared_Group == nullptr)
        return;

    const bool Is_Ready = Get_Local_Player_Ready_State();
    Shared_Group->Set_Local_Player_Ready(!Is_Ready); // ! Change it to the opposite of what it currently is
    Refresh_From_Shared_Elements();
}

void Game_Multiplayer_Lobby_Elements::Send_To_Chat() // sends msg to chat
{
    QString Message = Chat_Input->text().trimmed(); // cut off blank part

    if(Message.isEmpty())
        return;// dont send empty msgs

    qDebug() << "Message typed:" << Message;

    Add_Chat_Message("Me: " + Message);
    Chat_Input->clear();

    if(Networking_Group != nullptr)
    {
        qDebug() << "Networking exists";
        Networking_Group->Send_Chat_Message(Message); // send chat to network
    }
    else
    {
        qDebug() << "Networking is NULL";
    }
}

void Game_Multiplayer_Lobby_Elements::Handle_Leave_Lobby()//Resets local lobby data, clears chat text, and asks the menu manager to leave the lobby screen
{
    if(Shared_Group != nullptr)
    {
        Shared_Group->Reset_Lobby_State();
        Refresh_From_Shared_Elements();
    }

    Chat_Display->setPlainText(
                "The Void: The cosmos speaks through you.\n"
                "The Void: Chat will appear here.");

    emit Leave_Lobby_Requested();
}

void Game_Multiplayer_Lobby_Elements::Handle_Customize_Character()
{
    Add_Chat_Message("System: Character customization ");
}

void Game_Multiplayer_Lobby_Elements::Handle_Vote_Map()
{
    if(Difficulty_Popup != nullptr)
        Difficulty_Popup->setVisible(!Difficulty_Popup->isVisible());

    Add_Chat_Message("System: Pick a map difficulty.");
}

void Game_Multiplayer_Lobby_Elements::Handle_Easy_Difficulty()
{
    if(Shared_Group != nullptr)
    {
        if(Networking_Group != nullptr)
            Networking_Group->Send_Map_Vote("Easy");
        else
            Shared_Group->Add_Map_Difficulty_Vote("Easy");
        Tally_Difficulty_Votes();
        Add_Chat_Message("System: Easy vote added.");
    }
}

void Game_Multiplayer_Lobby_Elements::Handle_Medium_Difficulty()
{
    if(Shared_Group != nullptr)
    {
        if(Networking_Group != nullptr)
            Networking_Group->Send_Map_Vote("Medium");
        else
            Shared_Group->Add_Map_Difficulty_Vote("Medium");
        Tally_Difficulty_Votes();
        Add_Chat_Message("System: Medium vote added.");
    }
}

void Game_Multiplayer_Lobby_Elements::Handle_Hard_Difficulty()
{
    if(Shared_Group != nullptr)
    {
        if(Networking_Group != nullptr)
            Networking_Group->Send_Map_Vote("Hard");
        else
            Shared_Group->Add_Map_Difficulty_Vote("Hard");
        Tally_Difficulty_Votes();
        Add_Chat_Message("System: Hard vote added.");
    }
}

void Game_Multiplayer_Lobby_Elements::Tally_Difficulty_Votes()
{
    if(Shared_Group == nullptr)
        return;

    const QString Winner = Shared_Group->Get_Map_Difficulty();
    Difficulty_Current_Label->setText(QString("Selected: %1  E:%2 M:%3 H:%4")
                                      .arg(Winner)
                                      .arg(Shared_Group->Get_Easy_Votes())
                                      .arg(Shared_Group->Get_Medium_Votes())
                                      .arg(Shared_Group->Get_Hard_Votes()));
}

void Game_Multiplayer_Lobby_Elements::Handle_Apply_Lobby_Settings()
{
    if(Shared_Group == nullptr)
        return;

    Shared_Group->Set_Lobby_Privacy(Lobby_Public_Checkbox->isChecked());
    Shared_Group->Set_Max_Players(Lobby_Max_Players_Input->value());
    Shared_Group->Set_Lobby_Password(Lobby_Password_Input->text());
    Add_Chat_Message("System: Lobby settings updated.");
    Refresh_From_Shared_Elements();
}

void Game_Multiplayer_Lobby_Elements::Handle_Start_Game()
{
    Add_Chat_Message("System: Start game requested by host.");
    if(Networking_Group != nullptr)
        Networking_Group->Send_Game_Start(); // send req to whole lobby & save a copy
    emit Start_Game_Requested();
}

void Game_Multiplayer_Lobby_Elements::Handle_Edit_Lobby_Settings()
{
    if(Lobby_Settings_Popup != nullptr)
        Lobby_Settings_Popup->setVisible(!Lobby_Settings_Popup->isVisible());

    Add_Chat_Message("System: Edit lobby settings.");
}

void Game_Multiplayer_Lobby_Elements::Handle_Kick_Player()
{
    if(Shared_Group != nullptr)
    {
        Shared_Group->Remove_Last_Non_Host_Player();
        Add_Chat_Message("System: Last non-host player kicked.");
        Refresh_From_Shared_Elements();
    }
}

void Game_Multiplayer_Lobby_Elements::Rebuild_Player_Circles()
{
    if(Middle_Middle_Panel == nullptr || Middle_Middle_Panel->layout() == nullptr || Shared_Group == nullptr)
        return;

    if(Player_Circle_Holder != nullptr)
    {
        Middle_Middle_Panel->layout()->removeWidget(Player_Circle_Holder);
        delete Player_Circle_Holder;
        Player_Circle_Holder = nullptr;
    }

    Player_Circle_Labels.clear();

    QHBoxLayout *Circle_Row = new QHBoxLayout();
    Circle_Row->setSpacing(12);

    const QList<QColor> Colours =
    {
        QColor(90, 190, 255),
        QColor(255, 120, 140),
        QColor(130, 235, 150),
        QColor(245, 210, 80)
    };

    QList<Player_Info> Players = Shared_Group->Get_Player_List();

    for(int i = 0; i < Players.size(); i++)
    {
        QLabel *Circle = new QLabel(Middle_Middle_Panel);
        Circle->setFixedSize(44, 44);
        Circle->setAlignment(Qt::AlignCenter);
        Circle->setText(QString::number(i + 1));

        QColor Colour = Colours[i % Colours.size()];
        Circle->setStyleSheet(QString(
                                  "background-color: rgb(%1,%2,%3);"
                                  "border: 2px solid rgba(245,245,255,220);"
                                  "border-radius: 22px;"
                                  "color: rgb(15,20,30);"
                                  "font-weight: 700;")
                              .arg(Colour.red())
                              .arg(Colour.green())
                              .arg(Colour.blue()));

        Player_Circle_Labels.append(Circle);
        Circle_Row->addWidget(Circle);
    }

    Player_Circle_Holder = new QWidget(Middle_Middle_Panel);
    Player_Circle_Holder->setLayout(Circle_Row);
    Player_Circle_Holder->setStyleSheet("background: transparent; border: none;");
    Middle_Middle_Panel->layout()->addWidget(Player_Circle_Holder);
}
