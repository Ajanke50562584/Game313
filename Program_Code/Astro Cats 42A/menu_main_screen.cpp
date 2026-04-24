#include "menu_main_screen.h"

#include <QApplication>
#include <QFont>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QResizeEvent>
#include <QSizePolicy>
#include <QVBoxLayout>
#include <QFont>
#include <QPixmap>
#include <QDebug>
#include <QTimer>

Menu_Main_Screen::Menu_Main_Screen(QWidget *parent)
    : QWidget(parent)
{
    Main_Layout = new QVBoxLayout(this);


    setAttribute(Qt::WA_StyledBackground, true);
    QPixmap test(":/Images/Background/Space_BG_1.png");
    qDebug() << test.isNull();


    Sprite_Sheet = new QPixmap(":/Images/Characters/Awake_Laying_Orange.png");
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
    Character_Label->setAttribute(Qt::WA_TranslucentBackground);
    if (!Frames.isEmpty())
    {
        Character_Label->setPixmap(Frames[0]);
        qDebug() << "first frame size:" << Frames[0].size();
    }

    Menu_Character_Timer = new QTimer;
    connect(Menu_Character_Timer, &QTimer::timeout,this, &Menu_Main_Screen::Update_Frame);
    Menu_Character_Timer->start(150);


    setObjectName("MainMenu");
    setStyleSheet(
        "#MainMenu {"
        "border-image: url(:/Images/Background/Space_BG_1) 0 0 0 0 stretch stretch;"
        "}"
    );

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

QPushButton *Menu_Main_Screen::Get_Multiplayer_Button() const
{
    return Multiplayer_Button;
}

void Menu_Main_Screen::resizeEvent(QResizeEvent *event)
{
    const int w = width();

    QFont Title_Font("OCR A Extended");
    Title_Font.setPointSize(w / 40);
    Game_Title->setFont(Title_Font);

    QFont Push_Button_Font("OCR A Extended");
    Push_Button_Font.setPointSize(w / 65);
    for(int i = 0; i < Buttons.size(); i++)
        Buttons[i]->setFont(Push_Button_Font);

    if (Character_Label)
        {
            Character_Label->move(40, height() - Character_Label->height()-40);
            Character_Label->raise();
        }

    QWidget::resizeEvent(event);
}

void Menu_Main_Screen::Update_Frame()
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
