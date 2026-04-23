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
#include <QListWidget>
#include <QPushButton>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QResizeEvent>
#include <QSizePolicy>
#include <QSpinBox>
#include <QStackedWidget>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
//add
#include <QFont>
#include <QPixmap>
//added
namespace
{

// ================= MAIN MENU =================
class Main_Menu : public QWidget
{
public:
    explicit Main_Menu(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        //add
        setAttribute(Qt::WA_StyledBackground, true);
        //added

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


        //add
        QPixmap test(":/Images/Background/Space_BG_1.png");
        qDebug() << test.isNull();


        Sprite_Sheet = new QPixmap(":/Images/Charecters/Awake_Laying_Orange.png");
        qDebug() << "sprite null?" << Sprite_Sheet->isNull();
        qDebug() << "sprite size:" << Sprite_Sheet->width() << Sprite_Sheet->height();

        int frameWidth = 128;
        int frameHeight = 128;

        Frames.clear();
        for (int x = 0; x + frameWidth <= Sprite_Sheet->width(); x += frameWidth)
        {
            Frames.append(Sprite_Sheet->copy(x, 0, frameWidth, frameHeight));
        }
        qDebug() << "frames count:" << Frames.size();

        Character_Label = new QLabel(this);
        Character_Label->setFixedSize(384, 384);
        //Character_Label->setScaledContents(true);
        Character_Label->setAttribute(Qt::WA_TranslucentBackground);
        if (!Frames.isEmpty())
        {
            Character_Label->setPixmap(Frames[0]);
            qDebug() << "first frame size:" << Frames[0].size();
        }

        Menu_Character_Timer = new QTimer;
        connect(Menu_Character_Timer, &QTimer::timeout, this, &Main_Menu::Update_Frame);
        Menu_Character_Timer->start(150);


        setObjectName("MainMenu");
        setStyleSheet(
            "#MainMenu {"
            "border-image: url(:/Images/Background/Space_BG_1) 0 0 0 0 stretch stretch;"
            "}"
        );
        //added

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
        //add
        QFont Title_Font("OCR A Extended");
        Title_Font.setPointSize(w / 40);
        Game_Title->setFont(Title_Font);

        QFont Push_Button_Font;
        Push_Button_Font.setPointSize(w / 65);
        for(int i = 0; i < Buttons.size(); i++)
            Buttons[i]->setFont(Push_Button_Font);
        //add
        if (Character_Label)
            {
                Character_Label->move(40, height() - Character_Label->height()-40);
                Character_Label->raise();
            }
        //added

        QWidget::resizeEvent(event);
    }

private:
    int Current_Frame;
    QLabel *Game_Title;
    QVBoxLayout *Main_Layout;
    QList<QPushButton*> Buttons;
    QPushButton *Multiplayer_Button;
    //add
    QLabel *Character_Label;
    QList<QPixmap> Frames;
    QTimer *Menu_Character_Timer;
    QPixmap *Sprite_Sheet;

private slots:
    void Update_Frame()
    {
        if (Frames.isEmpty())
            return;
        QPixmap scaled = Frames[Current_Frame].scaled(
                Character_Label->size(),
                Qt::KeepAspectRatio,
                Qt::FastTransformation
            );
        Character_Label->setPixmap(Frames[Current_Frame]);
        Character_Label->setPixmap(scaled);

        Current_Frame = (Current_Frame + 1) % Frames.size();
        qDebug() << "Update_Frame called, current frame =" << Current_Frame;
    }


};

// ================= MULTIPLAYER MENU =================
class Multiplayer_Menu : public QWidget
{
public:
    explicit Multiplayer_Menu(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        Host_Button = new QPushButton("Become The Last \n Constellation ", this);
        Client_Button = new QPushButton("Traverse The Stars", this);
        Back_Button = new QPushButton("Return To The Void", this);

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

// ================= CREATE LOBBY =================
class Create_Lobby_Menu : public QWidget
{
public:
    explicit Create_Lobby_Menu(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        Main_Layout = new QVBoxLayout(this);
        Main_Layout->setContentsMargins(40, 25, 40, 25);
        Main_Layout->setSpacing(18);

        Title_Label = new QLabel("Create a Lobby", this);
        Title_Label->setAlignment(Qt::AlignCenter);
        Title_Label->setStyleSheet(
                    "color: rgb(235,235,245);"
                    "background-color: rgba(0, 74, 113, 95);"
                    "border: 2px solid rgba(200,200,230,120);"
                    "border-radius: 18px;"
                    "padding: 10px 20px;");

        Sub_Title_Label = new QLabel("Light the stars before the others arrive", this);
        Sub_Title_Label->setAlignment(Qt::AlignCenter);
        Sub_Title_Label->setStyleSheet(
                    "color: rgb(225,225,235);"
                    "background: transparent;"
                    "font-weight: 600;");

        Form_Widget = new QWidget(this);
        Form_Widget->setMaximumWidth(620);
        Form_Widget->setStyleSheet(
                    "background-color: rgba(70,78,92,210);"
                    "border: none;"
                    "border-radius: 24px;");

        Form_Layout = new QVBoxLayout(Form_Widget);
        Form_Layout->setContentsMargins(28, 24, 28, 24);
        Form_Layout->setSpacing(14);

        Lobby_Name_Label = new QLabel("Lobby Name", this);
        Max_Players_Label = new QLabel("Max Players", this);
        Password_Label = new QLabel("Lobby Password", this);

        const QString labelStyle =
                "QLabel {"
                "color: rgb(245,245,250);"
                "background: transparent;"
                "font-weight: 600;"
                "border: none;"
                "}";

        Lobby_Name_Label->setStyleSheet(labelStyle);
        Max_Players_Label->setStyleSheet(labelStyle);
        Password_Label->setStyleSheet(labelStyle);

        Lobby_Name_Input = new QLineEdit(this);
        Lobby_Name_Input->setMinimumHeight(50);
        Lobby_Name_Input->setPlaceholderText("Enter lobby name...");
        Lobby_Name_Input->setStyleSheet(
                    "QLineEdit {"
                    "background-color: rgb(165,170,180);"
                    "color: rgb(35,35,45);"
                    "border: 1px solid rgba(255,255,255,80);"
                    "border-radius: 16px;"
                    "padding: 10px 14px;"
                    "}"
                    "QLineEdit:focus {"
                    "border: 1px solid rgba(255,255,255,150);"
                    "}"
                    "QLineEdit:disabled {"
                    "background-color: rgba(120,120,130,90);"
                    "color: rgba(220,220,230,120);"
                    "border: 1px solid rgba(170,170,180,70);"
                    "}");

        Max_Players_Input = new QSpinBox(this);
        Max_Players_Input->setRange(2, 16);
        Max_Players_Input->setValue(4);
        Max_Players_Input->setMinimumHeight(50);
        Max_Players_Input->setStyleSheet(
                    "QSpinBox {"
                    "background-color: rgb(165,170,180);"
                    "color: rgb(35,35,45);"
                    "border: 1px solid rgba(255,255,255,80);"
                    "border-radius: 16px;"
                    "padding: 10px 14px;"
                    "}"
                    "QSpinBox:focus {"
                    "border: 1px solid rgba(255,255,255,150);"
                    "}");

        Public_Lobby_Checkbox = new QCheckBox("Public Lobby", this);
        Public_Lobby_Checkbox->setChecked(true);
        Public_Lobby_Checkbox->setCursor(Qt::PointingHandCursor);
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

        Password_Input = new QLineEdit(this);
        Password_Input->setMinimumHeight(50);
        Password_Input->setPlaceholderText("Enter password if private...");
        Password_Input->setEchoMode(QLineEdit::Password);
        Password_Input->setEnabled(false);
        Password_Input->setStyleSheet(
                    "QLineEdit {"
                    "background-color: rgb(165,170,180);"
                    "color: rgb(35,35,45);"
                    "border: 1px solid rgba(255,255,255,80);"
                    "border-radius: 16px;"
                    "padding: 10px 14px;"
                    "}"
                    "QLineEdit:focus {"
                    "border: 1px solid rgba(255,255,255,150);"
                    "}"
                    "QLineEdit:disabled {"
                    "background-color: rgba(120,120,130,90);"
                    "color: rgba(220,220,230,120);"
                    "border: 1px solid rgba(170,170,180,70);"
                    "}");

        Show_Password_Checkbox = new QCheckBox("Show Password", this);
        Show_Password_Checkbox->setCursor(Qt::PointingHandCursor);
        Show_Password_Checkbox->setEnabled(false);
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

        Create_Lobby_Button = new QPushButton("Create Lobby", this);
        Back_Button = new QPushButton("Back", this);

        const QString buttonStyle =
                "QPushButton {"
                "background-color: rgb(160,165,175);"
                "color: rgb(15,20,30);"
                "border: none;"
                "border-radius: 24px;"
                "padding: 12px 24px;"
                "font-weight: 600;"
                "}"
                "QPushButton:hover {"
                "background-color: rgb(180,185,195);"
                "}"
                "QPushButton:pressed {"
                "background-color: rgb(140,145,155);"
                "}";

        Create_Lobby_Button->setMinimumHeight(58);
        Back_Button->setMinimumHeight(58);

        Create_Lobby_Button->setStyleSheet(buttonStyle);
        Back_Button->setStyleSheet(buttonStyle);

        Create_Lobby_Button->setCursor(Qt::PointingHandCursor);
        Back_Button->setCursor(Qt::PointingHandCursor);

        connect(Create_Lobby_Button, &QPushButton::pressed, this, [=]() {
            Create_Lobby_Button->setCursor(Qt::ClosedHandCursor);
        });
        connect(Create_Lobby_Button, &QPushButton::released, this, [=]() {
            Create_Lobby_Button->setCursor(Qt::PointingHandCursor);
        });

        connect(Back_Button, &QPushButton::pressed, this, [=]() {
            Back_Button->setCursor(Qt::ClosedHandCursor);
        });
        connect(Back_Button, &QPushButton::released, this, [=]() {
            Back_Button->setCursor(Qt::PointingHandCursor);
        });

        Form_Layout->addWidget(Lobby_Name_Label);
        Form_Layout->addWidget(Lobby_Name_Input);
        Form_Layout->addSpacing(2);
        Form_Layout->addWidget(Max_Players_Label);
        Form_Layout->addWidget(Max_Players_Input);
        Form_Layout->addSpacing(2);
        Form_Layout->addWidget(Public_Lobby_Checkbox, 0, Qt::AlignLeft);
        Form_Layout->addSpacing(2);
        Form_Layout->addWidget(Password_Label);
        Form_Layout->addWidget(Password_Input);
        Form_Layout->addWidget(Show_Password_Checkbox, 0, Qt::AlignLeft);
        Form_Layout->addSpacing(16);

        QHBoxLayout *Button_Row = new QHBoxLayout();
        Button_Row->setSpacing(14);
        Button_Row->addWidget(Create_Lobby_Button);
        Button_Row->addWidget(Back_Button);

        Form_Layout->addLayout(Button_Row);

        QHBoxLayout *Title_Row = new QHBoxLayout();
        Title_Row->addStretch(1);
        Title_Row->addWidget(Title_Label, 3);
        Title_Row->addStretch(1);

        QHBoxLayout *Form_Row = new QHBoxLayout();
        Form_Row->addStretch(1);
        Form_Row->addWidget(Form_Widget, 0, Qt::AlignHCenter);
        Form_Row->addStretch(1);

        Main_Layout->addSpacing(5);
        Main_Layout->addLayout(Title_Row);
        Main_Layout->addWidget(Sub_Title_Label, 0, Qt::AlignHCenter);
        Main_Layout->addStretch(1);
        Main_Layout->addLayout(Form_Row);
        Main_Layout->addStretch(2);

        setStyleSheet("background: transparent;");

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
        Title_Font.setPointSize(qMax(18, w / 38));
        Title_Label->setFont(Title_Font);

        QFont Sub_Font;
        Sub_Font.setPointSize(qMax(10, w / 82));
        Sub_Title_Label->setFont(Sub_Font);

        QFont Label_Font;
        Label_Font.setPointSize(qMax(10, w / 84));
        Lobby_Name_Label->setFont(Label_Font);
        Max_Players_Label->setFont(Label_Font);
        Password_Label->setFont(Label_Font);
        Public_Lobby_Checkbox->setFont(Label_Font);
        Show_Password_Checkbox->setFont(Label_Font);

        QFont Input_Font;
        Input_Font.setPointSize(qMax(10, w / 88));
        Lobby_Name_Input->setFont(Input_Font);
        Max_Players_Input->setFont(Input_Font);
        Password_Input->setFont(Input_Font);
        Create_Lobby_Button->setFont(Input_Font);
        Back_Button->setFont(Input_Font);

        QWidget::resizeEvent(event);
    }

private:
    QVBoxLayout *Main_Layout;
    QWidget *Form_Widget;
    QVBoxLayout *Form_Layout;

    QLabel *Title_Label;
    QLabel *Sub_Title_Label;
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

// ================= JOIN SERVER MENU =================
class Join_Server_Menu : public QWidget
{
public:
    explicit Join_Server_Menu(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        Main_Layout = new QVBoxLayout(this);
        Main_Layout->setContentsMargins(40, 25, 40, 25);
        Main_Layout->setSpacing(18);

        Title_Label = new QLabel("Select Lobby", this);
        Title_Label->setAlignment(Qt::AlignCenter);
        Title_Label->setStyleSheet(
                    "color: rgb(235,235,245);"
                    "background-color: rgba(0, 74, 113, 95);"
                    "border: 2px solid rgba(200,200,230,120);"
                    "border-radius: 18px;"
                    "padding: 10px 20px;");

        Sub_Title_Label = new QLabel("Choose where your constellation begins", this);
        Sub_Title_Label->setAlignment(Qt::AlignCenter);
        Sub_Title_Label->setStyleSheet(
                    "color: rgb(225,225,235);"
                    "background: transparent;"
                    "font-weight: 600;");

        Card_Widget = new QWidget(this);
        Card_Widget->setMaximumWidth(620);
        Card_Widget->setStyleSheet(
                    "background-color: rgba(70,78,92,210);"
                    "border: none;"
                    "border-radius: 24px;");

        Card_Layout = new QVBoxLayout(Card_Widget);
        Card_Layout->setContentsMargins(28, 24, 28, 24);
        Card_Layout->setSpacing(16);

        Player_Name_Label = new QLabel("Player Name", this);
        Player_Name_Label->setStyleSheet(
                    "QLabel {"
                    "color: rgb(245,245,250);"
                    "background: transparent;"
                    "font-weight: 600;"
                    "border: none;"
                    "}");

        Player_Name_Input = new QLineEdit(this);
        Player_Name_Input->setMinimumHeight(50);
        Player_Name_Input->setMaxLength(16);
        Player_Name_Input->setPlaceholderText("Enter player name...");
        Player_Name_Input->setText("Player");
        Player_Name_Input->setStyleSheet(
                    "QLineEdit {"
                    "background-color: rgb(165,170,180);"
                    "color: rgb(35,35,45);"
                    "border: 1px solid rgba(255,255,255,80);"
                    "border-radius: 16px;"
                    "padding: 10px 14px;"
                    "}"
                    "QLineEdit:focus {"
                    "border: 1px solid rgba(255,255,255,150);"
                    "}");

        QRegularExpression Player_Name_Regex("[A-Za-z0-9_ ]{0,16}");
        Player_Name_Validator = new QRegularExpressionValidator(Player_Name_Regex, this);
        Player_Name_Input->setValidator(Player_Name_Validator);

        Lobby_List = new QListWidget(this);
        Lobby_List->setMinimumHeight(280);
        Lobby_List->setStyleSheet(
                    "QListWidget {"
                    "background-color: rgba(150,155,165,125);"
                    "color: rgb(245,245,250);"
                    "border: none;"
                    "border-radius: 18px;"
                    "padding: 12px;"
                    "}"
                    "QListWidget::item {"
                    "background-color: rgba(70,78,92,180);"
                    "border-radius: 14px;"
                    "padding: 14px;"
                    "margin: 6px;"
                    "}"
                    "QListWidget::item:selected {"
                    "background-color: rgba(120,180,255,180);"
                    "color: rgb(15,20,30);"
                    "}");

        Join_Button = new QPushButton("Join Lobby", this);
        Refresh_Button = new QPushButton("Refresh", this);
        Back_Button = new QPushButton("Back", this);

        const QString buttonStyle =
                "QPushButton {"
                "background-color: rgb(160,165,175);"
                "color: rgb(15,20,30);"
                "border: none;"
                "border-radius: 24px;"
                "padding: 12px 24px;"
                "font-weight: 600;"
                "}"
                "QPushButton:hover {"
                "background-color: rgb(180,185,195);"
                "}"
                "QPushButton:pressed {"
                "background-color: rgb(140,145,155);"
                "}";

        Join_Button->setMinimumHeight(58);
        Refresh_Button->setMinimumHeight(58);
        Back_Button->setMinimumHeight(58);

        Join_Button->setStyleSheet(buttonStyle);
        Refresh_Button->setStyleSheet(buttonStyle);
        Back_Button->setStyleSheet(buttonStyle);

        Join_Button->setCursor(Qt::PointingHandCursor);
        Refresh_Button->setCursor(Qt::PointingHandCursor);
        Back_Button->setCursor(Qt::PointingHandCursor);

        connect(Join_Button, &QPushButton::pressed, this, [=]() {
            Join_Button->setCursor(Qt::ClosedHandCursor);
        });
        connect(Join_Button, &QPushButton::released, this, [=]() {
            Join_Button->setCursor(Qt::PointingHandCursor);
        });

        connect(Refresh_Button, &QPushButton::pressed, this, [=]() {
            Refresh_Button->setCursor(Qt::ClosedHandCursor);
        });
        connect(Refresh_Button, &QPushButton::released, this, [=]() {
            Refresh_Button->setCursor(Qt::PointingHandCursor);
        });

        connect(Back_Button, &QPushButton::pressed, this, [=]() {
            Back_Button->setCursor(Qt::ClosedHandCursor);
        });
        connect(Back_Button, &QPushButton::released, this, [=]() {
            Back_Button->setCursor(Qt::PointingHandCursor);
        });

        Card_Layout->addWidget(Player_Name_Label);
        Card_Layout->addWidget(Player_Name_Input);
        Card_Layout->addWidget(Lobby_List);

        QHBoxLayout *Button_Row = new QHBoxLayout();
        Button_Row->setSpacing(14);
        Button_Row->addWidget(Join_Button);
        Button_Row->addWidget(Refresh_Button);
        Button_Row->addWidget(Back_Button);

        Card_Layout->addLayout(Button_Row);

        QHBoxLayout *Title_Row = new QHBoxLayout();
        Title_Row->addStretch(1);
        Title_Row->addWidget(Title_Label, 3);
        Title_Row->addStretch(1);

        QHBoxLayout *Card_Row = new QHBoxLayout();
        Card_Row->addStretch(1);
        Card_Row->addWidget(Card_Widget, 0, Qt::AlignHCenter);
        Card_Row->addStretch(1);

        Main_Layout->addSpacing(5);
        Main_Layout->addLayout(Title_Row);
        Main_Layout->addWidget(Sub_Title_Label, 0, Qt::AlignHCenter);
        Main_Layout->addStretch(1);
        Main_Layout->addLayout(Card_Row);
        Main_Layout->addStretch(2);

        setStyleSheet("background: transparent;");
    }

    QPushButton *Get_Join_Button() const { return Join_Button; }
    QPushButton *Get_Refresh_Button() const { return Refresh_Button; }
    QPushButton *Get_Back_Button() const { return Back_Button; }
    QListWidget *Get_Lobby_List() const { return Lobby_List; }
    QLineEdit *Get_Player_Name_Input() const { return Player_Name_Input; }

protected:
    void resizeEvent(QResizeEvent *event) override
    {
        const int w = width();

        QFont Title_Font;
        Title_Font.setPointSize(qMax(18, w / 38));
        Title_Label->setFont(Title_Font);

        QFont Sub_Font;
        Sub_Font.setPointSize(qMax(10, w / 82));
        Sub_Title_Label->setFont(Sub_Font);

        QFont Input_Font;
        Input_Font.setPointSize(qMax(10, w / 88));
        Player_Name_Label->setFont(Input_Font);
        Player_Name_Input->setFont(Input_Font);
        Lobby_List->setFont(Input_Font);
        Join_Button->setFont(Input_Font);
        Refresh_Button->setFont(Input_Font);
        Back_Button->setFont(Input_Font);

        QWidget::resizeEvent(event);
    }

private:
    QVBoxLayout *Main_Layout;
    QWidget *Card_Widget;
    QVBoxLayout *Card_Layout;
    QLabel *Title_Label;
    QLabel *Sub_Title_Label;
    QLabel *Player_Name_Label;
    QLineEdit *Player_Name_Input;
    QRegularExpressionValidator *Player_Name_Validator;
    QListWidget *Lobby_List;
    QPushButton *Join_Button;
    QPushButton *Refresh_Button;
    QPushButton *Back_Button;
};

}

// ================= GAME MENUES =================
Game_Menues::Game_Menues(QWidget *parent)
    : QObject(parent)
    , Shared_Group(nullptr)
    , Game_Networking_Group(nullptr)
    , Game_Multiplayer_Lobby_Group(nullptr)
    , Navigation_Is_Set_Up(false)
{
    // create stacked widget
    Screen_Stack = new QStackedWidget(parent);

    // create screens
    Main_Menu *Main_Menu_Screen = new Main_Menu(Screen_Stack);
    Multiplayer_Menu *Multiplayer_Menu_Screen = new Multiplayer_Menu(Screen_Stack);
    Create_Lobby_Menu *Create_Lobby_Menu_Screen = new Create_Lobby_Menu(Screen_Stack);
    Join_Server_Menu *Join_Server_Menu_Screen = new Join_Server_Menu(Screen_Stack);

    // store widgets
    Main_Menu_Widget = Main_Menu_Screen;
    Multiplayer_Menu_Widget = Multiplayer_Menu_Screen;
    Create_Lobby_Menu_Widget = Create_Lobby_Menu_Screen;
    Join_Server_Menu_Widget = Join_Server_Menu_Screen;

    // get button pointers
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

    // add widgets to stack
    Screen_Stack->addWidget(Main_Menu_Widget);
    Screen_Stack->addWidget(Multiplayer_Menu_Widget);
    Screen_Stack->addWidget(Create_Lobby_Menu_Widget);
    Screen_Stack->addWidget(Join_Server_Menu_Widget);
    Screen_Stack->setCurrentWidget(Main_Menu_Widget);
}

// attach shared elements
void Game_Menues::Attach_Shared_Elements(Shared_Elements *sharedElements)
{
    Shared_Group = sharedElements;
}

// attach networking elements
void Game_Menues::Attach_Networking_Elements(Game_Networking_Elements *networkingElements)
{
    Game_Networking_Group = networkingElements;
}

// attach lobby elements
void Game_Menues::Attach_Multiplayer_Lobby_Elements(Game_Multiplayer_Lobby_Elements *lobbyElements)
{
    Game_Multiplayer_Lobby_Group = lobbyElements;

    // add lobby widget if not already added
    if(Game_Multiplayer_Lobby_Group != nullptr)
    {
        QWidget *Lobby_Widget = Game_Multiplayer_Lobby_Group->Get_Lobby_Widget();

        if(Screen_Stack->indexOf(Lobby_Widget) == -1)
            Screen_Stack->addWidget(Lobby_Widget);
    }
}

// setup navigation
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
        Go_To_Join_Server_Menu();
    });

    connect(Create_Lobby_Back_Button, &QPushButton::clicked, this, [this]() {
        Go_To_Multiplayer_Menu();
    });

    connect(Create_Lobby_Button, &QPushButton::clicked, this, [this]() {
        Start_Hosting();
    });

    connect(Join_Server_Join_Button, &QPushButton::clicked, this, [this]() {
        Join_Game();
    });

    connect(Join_Server_Refresh_Button, &QPushButton::clicked, this, [this]() {

        if(Shared_Group == nullptr || Game_Networking_Group == nullptr || Join_Server_Lobby_List == nullptr)
            return;

        Game_Networking_Group->Start_Listening();

        Shared_Group->Clear_Discovered_Lobbies();
        Join_Server_Lobby_List->clear();

        Game_Networking_Group->Send_Discover_Lobbies();

        qDebug() << "Refreshing lobbies...";

        QTimer::singleShot(700, this, [this]() {

            if(Shared_Group == nullptr || Join_Server_Lobby_List == nullptr)
                return;

            Join_Server_Lobby_List->clear();

            QList<Lobby_Info> Lobbies = Shared_Group->Get_Discovered_Lobbies();

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

    connect(Join_Server_Back_Button, &QPushButton::clicked, this, [this]() {
        Go_To_Multiplayer_Menu();
    });

    Navigation_Is_Set_Up = true;
}

// get root widget
QWidget *Game_Menues::Get_Root_Widget() const
{
    return Screen_Stack;
}

// go to multiplayer menu
void Game_Menues::Go_To_Multiplayer_Menu()
{
    Screen_Stack->setCurrentWidget(Multiplayer_Menu_Widget);
}

// go to main menu
void Game_Menues::Go_To_Main_Menu()
{
    Screen_Stack->setCurrentWidget(Main_Menu_Widget);
}

// go to create lobby menu
void Game_Menues::Go_To_Create_Lobby_Menu()
{
    Screen_Stack->setCurrentWidget(Create_Lobby_Menu_Widget);
}

// go to join server menu
void Game_Menues::Go_To_Join_Server_Menu()
{
    Screen_Stack->setCurrentWidget(Join_Server_Menu_Widget);
}

// go to lobby menu
void Game_Menues::Go_To_Lobby_Menu()
{
    if(Game_Multiplayer_Lobby_Group != nullptr)
        Screen_Stack->setCurrentWidget(Game_Multiplayer_Lobby_Group->Get_Lobby_Widget());
}

// start hosting
void Game_Menues::Start_Hosting()
{
    if(Shared_Group == nullptr || Game_Networking_Group == nullptr || Game_Multiplayer_Lobby_Group == nullptr)
    {
        qDebug() << "Missing groups";
        return;
    }

    // start fresh
    Shared_Group->Reset_Lobby_State();

    // copy form values into shared state
    Push_Create_Lobby_Form_Data_Into_Shared_Elements();

    // mark this as host state
    Shared_Group->Set_Is_Hosting(true);
    Shared_Group->Set_Is_In_Lobby(true);
    Shared_Group->Set_Player_Role("Host");
    Shared_Group->Set_Connection_Status("Hosting lobby");
    Shared_Group->Set_Last_Network_Event("Host created lobby");

    // add the host into the player list
    Player_Info Host_Player;
    Host_Player.Player_ID = 1;
    Host_Player.Player_Name = Shared_Group->Get_Local_Player_Name();
    Host_Player.Is_Host = true;
    Host_Player.Is_Ready = true;

    Shared_Group->Clear_Players();
    Shared_Group->Add_Player(Host_Player);

    // start listening
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

// join game
void Game_Menues::Join_Game()
{
    if(Shared_Group == nullptr || Game_Networking_Group == nullptr || Game_Multiplayer_Lobby_Group == nullptr)
    {
        qDebug() << "Missing groups";
        return;
    }

    // need a selected lobby
    if(Join_Server_Lobby_List == nullptr || Join_Server_Lobby_List->currentItem() == nullptr)
    {
        qDebug() << "No lobby selected";
        return;
    }

    int Selected_Row = Join_Server_Lobby_List->currentRow();

    QList<Lobby_Info> Lobbies = Shared_Group->Get_Discovered_Lobbies();

    // make sure selected row is valid
    if(Selected_Row < 0 || Selected_Row >= Lobbies.size())
    {
        qDebug() << "Selected row out of range";
        return;
    }

    Lobby_Info Selected_Lobby = Lobbies[Selected_Row];

    // reset old state
    Shared_Group->Reset_Lobby_State();

    // get player name
    QString Player_Name = "Player";
    if(Join_Server_Player_Name_Input != nullptr)
    {
        QString Entered_Name = Join_Server_Player_Name_Input->text().trimmed();

        if(!Entered_Name.isEmpty())
            Player_Name = Entered_Name;
    }

    // store lobby + player info
    Shared_Group->Set_Local_Player_Name(Player_Name);
    Shared_Group->Set_Selected_Lobby(Selected_Lobby);
    Shared_Group->Set_Lobby_Name(Selected_Lobby.Lobby_Name);
    Shared_Group->Set_Player_Role("Client");
    Shared_Group->Set_Is_Hosting(false);
    Shared_Group->Set_Is_In_Lobby(true);
    Shared_Group->Set_Remote_Address(Selected_Lobby.Host_Address.toString() + ":" +
                                     QString::number(Selected_Lobby.Host_Port));
    Shared_Group->Set_Last_Network_Event("Joining selected lobby");

    // client also needs to listen
    Game_Networking_Group->Start_Listening();

    // send join request to the actual selected host
    Game_Networking_Group->Request_To_Join(Selected_Lobby.Host_Address,
                                           Selected_Lobby.Host_Port);

    Game_Multiplayer_Lobby_Group->Refresh_From_Shared_Elements();
    Go_To_Lobby_Menu();
}

// push create lobby data into shared elements
void Game_Menues::Push_Create_Lobby_Form_Data_Into_Shared_Elements()
{
    if(Shared_Group == nullptr)
        return;

    // push create form data into shared state
    Shared_Group->Set_Lobby_Name(Lobby_Name_Input->text());
    Shared_Group->Set_Max_Players(Max_Players_Input->value());
    Shared_Group->Set_Lobby_Privacy(Public_Lobby_Checkbox->isChecked());

    // host defaults
    Shared_Group->Set_Is_In_Lobby(true);
    Shared_Group->Set_Local_Player_Name("Host");
    Shared_Group->Set_Local_Player_ID(1);
}

