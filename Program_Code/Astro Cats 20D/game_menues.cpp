#include "game_menues.h"

#include "game_multiplayer_lobby_elements.h"
#include "game_networking_elements.h"
#include "shared_elements.h"

#include <QApplication>
#include <QCheckBox>
#include <QDebug>
#include <QFont>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHostAddress>
#include <QLabel>
#include <QLineEdit>
#include <QList>
#include <QPushButton>
#include <QResizeEvent>
#include <QSizePolicy>
#include <QSpinBox>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWidget>

namespace
{
class Main_Menu : public QWidget
{
public:
    explicit Main_Menu(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        Main_Layout = new QVBoxLayout(this);

        Game_Title = new QLabel("Astro-Cats: <br>  The Last Constellation", this);
        Game_Title->setAlignment(Qt::AlignCenter);
        Game_Title->setWordWrap(true);
        Game_Title->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

        QPushButton *Story_Mode_Button = new QPushButton("Story Mode", this);
        Multiplayer_Button = new QPushButton("Multi Player", this);
        QPushButton *View_Highscore_Button = new QPushButton("View Highscores", this);
        QPushButton *Exit_Game_Button = new QPushButton("Exit Game", this);

        Buttons.append(Story_Mode_Button);
        Buttons.append(Multiplayer_Button);
        Buttons.append(View_Highscore_Button);
        Buttons.append(Exit_Game_Button);

        for(int i = 0; i < Buttons.size(); i++)
        {
            Buttons[i]->setMinimumHeight(55);
            Buttons[i]->setMaximumWidth(500);
            Buttons[i]->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
            Buttons[i]->setStyleSheet(
                        "QPushButton {"
                        "background-color: rgba(210,210,220,135);"
                        "color: rgb(20,20,30);"
                        "border: 2px solid rgba(245,245,255,190);"
                        "border-radius: 26px;"
                        "padding: 10px 24px;"
                        "}"
                        "QPushButton:hover {"
                        "background-color: rgba(235,235,245,175);"
                        "border: 2px solid rgba(255,255,255,230);"
                        "}"
                        "QPushButton:pressed {"
                        "background-color: rgba(180,180,195,190);"
                        "}");
        }

        setStyleSheet("background: transparent;");

        Game_Title->setStyleSheet(
                    "color: rgb(235,235,245);"
                    "background-color: rgba(0, 74, 113, 237);"
                    "border: 2px solid rgba(200,200,230,120);"
                    "border-radius: 18px;"
                    "padding: 10px 20px;");

        QHBoxLayout *Title_Row = new QHBoxLayout();
        Title_Row->addStretch(1);
        Title_Row->addWidget(Game_Title, 3);
        Title_Row->addStretch(1);

        Main_Layout->addLayout(Title_Row);
        Main_Layout->addSpacing(20);
        Main_Layout->addStretch(1);

        for(int i = 0; i < Buttons.size(); i++)
        {
            Main_Layout->addWidget(Buttons[i], 0, Qt::AlignHCenter);
            Buttons[i]->setCursor(Qt::PointingHandCursor);

            connect(Buttons[i], &QPushButton::pressed, this, [=]() {
                Buttons[i]->setCursor(Qt::ClosedHandCursor);
            });

            connect(Buttons[i], &QPushButton::released, this, [=]() {
                Buttons[i]->setCursor(Qt::PointingHandCursor);
            });
        }

        Main_Layout->addStretch(2);
        connect(Exit_Game_Button, &QPushButton::clicked, qApp, &QApplication::quit);
    }

    QPushButton *Get_Multiplayer_Button() const
    {
        return Multiplayer_Button;
    }

protected:
    void resizeEvent(QResizeEvent *event) override
    {
        const int w = width();

        QFont Title_Font;
        Title_Font.setPointSize(w / 40);
        Game_Title->setFont(Title_Font);

        QFont Push_Button_Font;
        Push_Button_Font.setPointSize(w / 65);
        for(int i = 0; i < Buttons.size(); i++)
            Buttons[i]->setFont(Push_Button_Font);

        QWidget::resizeEvent(event);
    }

private:
    QLabel *Game_Title;
    QVBoxLayout *Main_Layout;
    QList<QPushButton*> Buttons;
    QPushButton *Multiplayer_Button;
};

class Multiplayer_Menu : public QWidget
{
public:
    explicit Multiplayer_Menu(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        Host_Button = new QPushButton("Become the Cosmos", this);
        Client_Button = new QPushButton("Enter the Cosmos", this);
        Back_Button = new QPushButton("Return to the Void", this);

        Host_Button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        Client_Button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        Back_Button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        Host_Button->setCursor(Qt::PointingHandCursor);
        Client_Button->setCursor(Qt::PointingHandCursor);
        Back_Button->setCursor(Qt::PointingHandCursor);

        auto setClosed = [=](QPushButton *button) { button->setCursor(Qt::ClosedHandCursor); };
        auto setPointing = [=](QPushButton *button) { button->setCursor(Qt::PointingHandCursor); };

        connect(Host_Button, &QPushButton::pressed, this, [=]() { setClosed(Host_Button); });
        connect(Client_Button, &QPushButton::pressed, this, [=]() { setClosed(Client_Button); });
        connect(Back_Button, &QPushButton::pressed, this, [=]() { setClosed(Back_Button); });

        connect(Host_Button, &QPushButton::released, this, [=]() { setPointing(Host_Button); });
        connect(Client_Button, &QPushButton::released, this, [=]() { setPointing(Client_Button); });
        connect(Back_Button, &QPushButton::released, this, [=]() { setPointing(Back_Button); });

        Host_Button->setMinimumSize(180, 120);
        Client_Button->setMinimumSize(180, 120);
        Back_Button->setMinimumSize(180, 120);

        Host_Button->setStyleSheet(
            "QPushButton {"
            "background-color: rgba(255, 240, 160, 180);"
            "border: 2px solid white;"
            "border-radius: 20px;"
            "font-size: 22px;"
            "font-weight: bold;"
            "padding: 20px;"
            "}");

        Client_Button->setStyleSheet(
            "QPushButton {"
            "background-color: rgba(120, 180, 255, 180);"
            "border: 2px solid white;"
            "border-radius: 20px;"
            "font-size: 22px;"
            "font-weight: bold;"
            "padding: 20px;"
            "}");

        Back_Button->setStyleSheet(
            "QPushButton {"
            "background-color: rgba(40, 40, 40, 220);"
            "color: white;"
            "border: 2px solid white;"
            "border-radius: 20px;"
            "font-size: 22px;"
            "font-weight: bold;"
            "padding: 20px;"
            "}");

        QGridLayout *Layout = new QGridLayout(this);
        Layout->addWidget(Host_Button, 0, 0, Qt::AlignTop | Qt::AlignLeft);
        Layout->addWidget(Back_Button, 1, 1, Qt::AlignCenter);
        Layout->addWidget(Client_Button, 2, 2, Qt::AlignBottom | Qt::AlignRight);

        Layout->setRowStretch(0, 1);
        Layout->setRowStretch(1, 1);
        Layout->setRowStretch(2, 1);
        Layout->setColumnStretch(0, 1);
        Layout->setColumnStretch(1, 1);
        Layout->setColumnStretch(2, 1);
        Layout->setContentsMargins(40, 40, 40, 40);
        Layout->setSpacing(30);
        setLayout(Layout);
    }

    QPushButton *Get_Host_Button() const { return Host_Button; }
    QPushButton *Get_Client_Button() const { return Client_Button; }
    QPushButton *Get_Back_Button() const { return Back_Button; }

private:
    QPushButton *Host_Button;
    QPushButton *Client_Button;
    QPushButton *Back_Button;
};

class Create_Lobby_Menu : public QWidget
{
public:
    explicit Create_Lobby_Menu(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        Main_Layout = new QVBoxLayout(this);
        Form_Widget = new QWidget(this);
        Form_Layout = new QVBoxLayout(Form_Widget);

        Title_Label = new QLabel("Create a Lobby", this);
        Lobby_Name_Label = new QLabel("Lobby Name", this);
        Lobby_Name_Input = new QLineEdit(this);
        Max_Players_Label = new QLabel("Max Players", this);
        Max_Players_Input = new QSpinBox(this);
        Public_Lobby_Checkbox = new QCheckBox("Public Lobby", this);
        Password_Label = new QLabel("Lobby Password", this);
        Password_Input = new QLineEdit(this);
        Show_Password_Checkbox = new QCheckBox("Show Password", this);
        Create_Lobby_Button = new QPushButton("Create Lobby", this);
        Back_Button = new QPushButton("Back", this);

        Max_Players_Input->setRange(2, 16);
        Max_Players_Input->setValue(4);
        Password_Input->setEchoMode(QLineEdit::Password);
        Public_Lobby_Checkbox->setChecked(true);

        setStyleSheet("background: transparent;");

        Title_Label->setAlignment(Qt::AlignCenter);
        Title_Label->setStyleSheet(
                    "color: rgb(235,235,245);"
                    "background-color: rgba(0, 74, 113, 237);"
                    "border: 2px solid rgba(200,200,230,120);"
                    "border-radius: 18px;"
                    "padding: 10px 20px;");

        Form_Widget->setMaximumWidth(520);
        Form_Widget->setStyleSheet(
                    "background-color: rgba(10,20,35,180);"
                    "border: 2px solid rgba(200,200,230,100);"
                    "border-radius: 22px;");

        const QString labelStyle =
                "QLabel {"
                "color: rgb(235,235,245);"
                "background: transparent;"
                "font-size: 18px;"
                "font-weight: bold;"
                "}";

        Lobby_Name_Label->setStyleSheet(labelStyle);
        Max_Players_Label->setStyleSheet(labelStyle);
        Password_Label->setStyleSheet(labelStyle);

        Public_Lobby_Checkbox->setStyleSheet(
                    "QCheckBox {"
                    "color: rgb(235,235,245);"
                    "spacing: 10px;"
                    "background: transparent;"
                    "}"
                    "QCheckBox::indicator {"
                    "width: 20px;"
                    "height: 20px;"
                    "}");

        Show_Password_Checkbox->setStyleSheet(
                    "QCheckBox {"
                    "color: rgb(235,235,245);"
                    "spacing: 10px;"
                    "background: transparent;"
                    "}"
                    "QCheckBox::indicator {"
                    "width: 20px;"
                    "height: 20px;"
                    "}");

        Lobby_Name_Input->setMinimumHeight(48);
        Lobby_Name_Input->setPlaceholderText("Enter lobby name...");
        Lobby_Name_Input->setStyleSheet(
                    "QLineEdit {"
                    "background-color: rgba(210,210,220,135);"
                    "color: rgb(20,20,30);"
                    "border: 2px solid rgba(245,245,255,190);"
                    "border-radius: 16px;"
                    "padding: 8px 14px;"
                    "}"
                    "QLineEdit:focus {"
                    "background-color: rgba(235,235,245,175);"
                    "border: 2px solid rgba(255,255,255,230);"
                    "}");

        Password_Input->setMinimumHeight(48);
        Password_Input->setPlaceholderText("Enter password if private...");
        Password_Input->setEnabled(false);
        Password_Input->setStyleSheet(
                    "QLineEdit {"
                    "background-color: rgba(210,210,220,135);"
                    "color: rgb(20,20,30);"
                    "border: 2px solid rgba(245,245,255,190);"
                    "border-radius: 16px;"
                    "padding: 8px 14px;"
                    "}"
                    "QLineEdit:focus {"
                    "background-color: rgba(235,235,245,175);"
                    "border: 2px solid rgba(255,255,255,230);"
                    "}"
                    "QLineEdit:disabled {"
                    "background-color: rgba(120,120,130,90);"
                    "color: rgba(220,220,230,120);"
                    "border: 2px solid rgba(170,170,180,90);"
                    "}");

        Max_Players_Input->setMinimumHeight(48);
        Max_Players_Input->setStyleSheet(
                    "QSpinBox {"
                    "background-color: rgba(210,210,220,135);"
                    "color: rgb(20,20,30);"
                    "border: 2px solid rgba(245,245,255,190);"
                    "border-radius: 16px;"
                    "padding: 8px 14px;"
                    "}"
                    "QSpinBox:focus {"
                    "background-color: rgba(235,235,245,175);"
                    "border: 2px solid rgba(255,255,255,230);"
                    "}");

        const QString buttonStyle =
                "QPushButton {"
                "background-color: rgba(210,210,220,135);"
                "color: rgb(20,20,30);"
                "border: 2px solid rgba(245,245,255,190);"
                "border-radius: 26px;"
                "padding: 10px 24px;"
                "}"
                "QPushButton:hover {"
                "background-color: rgba(235,235,245,175);"
                "border: 2px solid rgba(255,255,255,230);"
                "}"
                "QPushButton:pressed {"
                "background-color: rgba(180,180,195,190);"
                "}";

        Create_Lobby_Button->setMinimumHeight(58);
        Create_Lobby_Button->setMinimumWidth(240);
        Create_Lobby_Button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        Create_Lobby_Button->setStyleSheet(buttonStyle);

        Back_Button->setMinimumHeight(58);
        Back_Button->setMinimumWidth(240);
        Back_Button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        Back_Button->setStyleSheet(buttonStyle);

        Main_Layout->setSpacing(20);

        QHBoxLayout *Title_Row = new QHBoxLayout();
        Title_Row->addStretch(1);
        Title_Row->addWidget(Title_Label, 3);
        Title_Row->addStretch(1);

        Form_Layout->setSpacing(14);
        Form_Layout->addWidget(Lobby_Name_Label);
        Form_Layout->addWidget(Lobby_Name_Input);
        Form_Layout->addSpacing(4);
        Form_Layout->addWidget(Max_Players_Label);
        Form_Layout->addWidget(Max_Players_Input);
        Form_Layout->addSpacing(4);
        Form_Layout->addWidget(Public_Lobby_Checkbox, 0, Qt::AlignLeft);
        Form_Layout->addSpacing(4);
        Form_Layout->addWidget(Password_Label);
        Form_Layout->addWidget(Password_Input);
        Form_Layout->addWidget(Show_Password_Checkbox, 0, Qt::AlignLeft);
        Form_Layout->addSpacing(20);
        Form_Layout->addWidget(Create_Lobby_Button, 0, Qt::AlignHCenter);
        Form_Layout->addWidget(Back_Button, 0, Qt::AlignHCenter);

        Main_Layout->addSpacing(20);
        Main_Layout->addLayout(Title_Row);
        Main_Layout->addSpacing(30);
        Main_Layout->addWidget(Form_Widget, 0, Qt::AlignHCenter);
        Main_Layout->addStretch(1);

        Create_Lobby_Button->setCursor(Qt::PointingHandCursor);
        Back_Button->setCursor(Qt::PointingHandCursor);
        Public_Lobby_Checkbox->setCursor(Qt::PointingHandCursor);
        Show_Password_Checkbox->setCursor(Qt::PointingHandCursor);

        connect(Public_Lobby_Checkbox, &QCheckBox::toggled, this, [this](bool) {
            if(Public_Lobby_Checkbox->isChecked())
            {
                Password_Input->setEnabled(false);
                Show_Password_Checkbox->setEnabled(false);
                Show_Password_Checkbox->setChecked(false);
                Password_Input->setEchoMode(QLineEdit::Password);
            }
            else
            {
                Password_Input->setEnabled(true);
                Show_Password_Checkbox->setEnabled(true);
            }
        });

        connect(Show_Password_Checkbox, &QCheckBox::toggled, this, [this](bool checked) {
            Password_Input->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
        });
    }

    QPushButton *Get_Create_Lobby_Button() const { return Create_Lobby_Button; }
    QPushButton *Get_Back_Button() const { return Back_Button; }
    QLineEdit *Get_Lobby_Name_Input() const { return Lobby_Name_Input; }
    QSpinBox *Get_Max_Players_Input() const { return Max_Players_Input; }
    QCheckBox *Get_Public_Lobby_Checkbox() const { return Public_Lobby_Checkbox; }

protected:
    void resizeEvent(QResizeEvent *event) override
    {
        const int w = width();

        QFont Title_Font;
        Title_Font.setPointSize(w / 38);
        Title_Label->setFont(Title_Font);

        QFont Label_Font;
        Label_Font.setPointSize(w / 70);
        Lobby_Name_Label->setFont(Label_Font);
        Max_Players_Label->setFont(Label_Font);
        Password_Label->setFont(Label_Font);
        Public_Lobby_Checkbox->setFont(Label_Font);
        Show_Password_Checkbox->setFont(Label_Font);

        QFont Input_Font;
        Input_Font.setPointSize(w / 75);
        Lobby_Name_Input->setFont(Input_Font);
        Password_Input->setFont(Input_Font);
        Max_Players_Input->setFont(Input_Font);
        Create_Lobby_Button->setFont(Input_Font);
        Back_Button->setFont(Input_Font);

        QWidget::resizeEvent(event);
    }

private:
    QVBoxLayout *Main_Layout;
    QWidget *Form_Widget;
    QVBoxLayout *Form_Layout;

    QLabel *Title_Label;
    QLabel *Lobby_Name_Label;
    QLineEdit *Lobby_Name_Input;
    QLabel *Max_Players_Label;
    QSpinBox *Max_Players_Input;
    QCheckBox *Public_Lobby_Checkbox;
    QLabel *Password_Label;
    QLineEdit *Password_Input;
    QCheckBox *Show_Password_Checkbox;
    QPushButton *Create_Lobby_Button;
    QPushButton *Back_Button;
};
}

Game_Menues::Game_Menues(QWidget *parent)
    : QObject(parent)
    , Shared_Group(nullptr)
    , Game_Networking_Group(nullptr)
    , Game_Multiplayer_Lobby_Group(nullptr)
    , Navigation_Is_Set_Up(false)
{
    Screen_Stack = new QStackedWidget(parent);

    Main_Menu *Main_Menu_Screen = new Main_Menu(Screen_Stack);
    Multiplayer_Menu *Multiplayer_Menu_Screen = new Multiplayer_Menu(Screen_Stack);
    Create_Lobby_Menu *Create_Lobby_Menu_Screen = new Create_Lobby_Menu(Screen_Stack);

    Main_Menu_Widget = Main_Menu_Screen;
    Multiplayer_Menu_Widget = Multiplayer_Menu_Screen;
    Create_Lobby_Menu_Widget = Create_Lobby_Menu_Screen;

    Main_Menu_Multiplayer_Button = Main_Menu_Screen->Get_Multiplayer_Button();
    Multiplayer_Host_Button = Multiplayer_Menu_Screen->Get_Host_Button();
    Multiplayer_Client_Button = Multiplayer_Menu_Screen->Get_Client_Button();
    Multiplayer_Back_Button = Multiplayer_Menu_Screen->Get_Back_Button();
    Create_Lobby_Button = Create_Lobby_Menu_Screen->Get_Create_Lobby_Button();
    Create_Lobby_Back_Button = Create_Lobby_Menu_Screen->Get_Back_Button();
    Lobby_Name_Input = Create_Lobby_Menu_Screen->Get_Lobby_Name_Input();
    Max_Players_Input = Create_Lobby_Menu_Screen->Get_Max_Players_Input();
    Public_Lobby_Checkbox = Create_Lobby_Menu_Screen->Get_Public_Lobby_Checkbox();

    Screen_Stack->addWidget(Main_Menu_Widget);
    Screen_Stack->addWidget(Multiplayer_Menu_Widget);
    Screen_Stack->addWidget(Create_Lobby_Menu_Widget);
    Screen_Stack->setCurrentWidget(Main_Menu_Widget);
}

void Game_Menues::Attach_Shared_Elements(Shared_Elements *sharedElements)
{
    Shared_Group = sharedElements;
}

void Game_Menues::Attach_Networking_Elements(Game_Networking_Elements *networkingElements)
{
    Game_Networking_Group = networkingElements;
}

void Game_Menues::Attach_Multiplayer_Lobby_Elements(Game_Multiplayer_Lobby_Elements *lobbyElements)
{
    Game_Multiplayer_Lobby_Group = lobbyElements;

    if(Game_Multiplayer_Lobby_Group != nullptr)
    {
        QWidget *Lobby_Widget = Game_Multiplayer_Lobby_Group->Get_Lobby_Widget();
        if(Screen_Stack->indexOf(Lobby_Widget) == -1)
            Screen_Stack->addWidget(Lobby_Widget);
    }
}

void Game_Menues::Setup_Navigation()
{
    if(Navigation_Is_Set_Up)
        return;

    connect(Main_Menu_Multiplayer_Button, &QPushButton::clicked, this, [this]() {
        Go_To_Multiplayer_Menu();
    });

    connect(Multiplayer_Back_Button, &QPushButton::clicked, this, [this]() {
        Go_To_Main_Menu();
    });

    connect(Multiplayer_Host_Button, &QPushButton::clicked, this, [this]() {
        Go_To_Create_Lobby_Menu();
    });

    connect(Multiplayer_Client_Button, &QPushButton::clicked, this, [this]() {
        Join_Game();
    });

    connect(Create_Lobby_Back_Button, &QPushButton::clicked, this, [this]() {
        Go_To_Multiplayer_Menu();
    });

    connect(Create_Lobby_Button, &QPushButton::clicked, this, [this]() {
        Start_Hosting();
    });

    Navigation_Is_Set_Up = true;
}

QWidget *Game_Menues::Get_Root_Widget() const
{
    return Screen_Stack;
}

void Game_Menues::Go_To_Multiplayer_Menu()
{
    Screen_Stack->setCurrentWidget(Multiplayer_Menu_Widget);
}

void Game_Menues::Go_To_Main_Menu()
{
    Screen_Stack->setCurrentWidget(Main_Menu_Widget);
}

void Game_Menues::Go_To_Create_Lobby_Menu()
{
    Screen_Stack->setCurrentWidget(Create_Lobby_Menu_Widget);
}

void Game_Menues::Go_To_Lobby_Menu()
{
    if(Game_Multiplayer_Lobby_Group != nullptr)
        Screen_Stack->setCurrentWidget(Game_Multiplayer_Lobby_Group->Get_Lobby_Widget());
}

void Game_Menues::Start_Hosting()
{
    if(Shared_Group == nullptr || Game_Networking_Group == nullptr || Game_Multiplayer_Lobby_Group == nullptr)
    {
        qDebug() << "Shared, networking or lobby group not attached";
        return;
    }

    Push_Create_Lobby_Form_Data_Into_Shared_Elements();
    Shared_Group->Set_Player_Role("Host");
    Shared_Group->Set_Last_Network_Event("Create lobby pressed");

    if(Game_Networking_Group->Start_Listening())
    {
        qDebug() << "Hosting started successfully";
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
    if(Shared_Group == nullptr || Game_Networking_Group == nullptr || Game_Multiplayer_Lobby_Group == nullptr)
    {
        qDebug() << "Shared, networking or lobby group not attached";
        return;
    }

    Shared_Group->Reset_Lobby_State();
    Shared_Group->Set_Lobby_Name("Joined Lobby");
    Shared_Group->Set_Player_Role("Client");
    Shared_Group->Set_Last_Network_Event("Client selected Enter the Cosmos");

    qDebug() << "Joining game";
    Game_Networking_Group->Request_To_Join(QHostAddress::LocalHost, 30033);
    Game_Multiplayer_Lobby_Group->Refresh_From_Shared_Elements();
    Go_To_Lobby_Menu();
}

void Game_Menues::Push_Create_Lobby_Form_Data_Into_Shared_Elements()
{
    if(Shared_Group == nullptr)
        return;

    Shared_Group->Set_Lobby_Name(Lobby_Name_Input->text());
    Shared_Group->Set_Max_Players(Max_Players_Input->value());
    Shared_Group->Set_Lobby_Privacy(Public_Lobby_Checkbox->isChecked());
    Shared_Group->Set_Is_In_Lobby(true);
}
