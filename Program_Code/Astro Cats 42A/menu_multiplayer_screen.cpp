#include "menu_multiplayer_screen.h"

#include <QGridLayout>
#include <QPushButton>
#include <QSizePolicy>
#include <QFont>
#include <QPixmap>
#include <QDebug>
#include <QTimer>

Menu_Multiplayer_Screen::Menu_Multiplayer_Screen(QWidget *parent)
    : QWidget(parent)
{

    Current_Frame = 0;
    setAttribute(Qt::WA_StyledBackground, true);
    QPixmap test(":/Images/Background/Space_BG_2.png");
    qDebug() << test.isNull();

    setObjectName("MultiplayerMenu");
    setStyleSheet(
        "#MultiplayerMenu {"
        "border-image: url(:/Images/Background/Space_BG_2.png) 0 0 0 0 stretch stretch;"
        "}"
    );

    Sprite_Sheet = new QPixmap(":/Images/Characters/Sitting_Black.png");
    qDebug() << "sprite null?" << Sprite_Sheet->isNull();
    qDebug() << "sprite size:" << Sprite_Sheet->width() << Sprite_Sheet->height();

    if (!Frames.isEmpty())
    {
        Character_Label->setPixmap(
            Frames[0].scaled(Character_Label->size(),
                             Qt::KeepAspectRatio,
                             Qt::FastTransformation)
        );
        Character_Label->show();
        qDebug() << "first frame size:" << Frames[0].size();
    }

//    int Frame_Width = 32;
//    int Frame_Height = 32;

//    Frames.clear();
//    for (int x = 0; x + Frame_Width <= Sprite_Sheet->width(); x += Frame_Width)
//    {
//        Frames.append(Sprite_Sheet->copy(x, 0, Frame_Width, Frame_Height));
//    }
    int Frame_Width = Sprite_Sheet->width() / 8;
    int Frame_Height = Sprite_Sheet->height();

    Frames.clear();
    for (int x = 0; x + Frame_Width <= Sprite_Sheet->width(); x += Frame_Width)
    {
        Frames.append(Sprite_Sheet->copy(x, 0, Frame_Width, Frame_Height));
    }
    qDebug() << "frames count:" << Frames.size();

    Character_Label = new QLabel(this);
    Character_Label->setFixedSize(96, 96);
    Character_Label->setAttribute(Qt::WA_TranslucentBackground);
    Character_Label->move(30, 60);
    Character_Label->raise();

    if (!Frames.isEmpty())
    {
        Character_Label->setPixmap(Frames[0]);
        qDebug() << "first frame size:" << Frames[0].size();
    }

    Multiplayer_Character_Timer = new QTimer(this);
    connect(Multiplayer_Character_Timer, &QTimer::timeout,this, &Menu_Multiplayer_Screen::Update_Frame);
    Multiplayer_Character_Timer->start(150);

    Host_Button = new QPushButton("Become The Last\nConstellation", this);
    Client_Button = new QPushButton("Traverse The Stars", this);
    Back_Button = new QPushButton("Return To The Void", this);

    //make all da buttons same size so da menu looks even
    Host_Button->setFixedSize(260, 120);
    Client_Button->setFixedSize(260, 120);
    Back_Button->setFixedSize(260, 120);

    Host_Button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    Client_Button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    Back_Button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

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

    Host_Button->setStyleSheet(
        "QPushButton {"
        "background-color: rgba(120, 180, 255, 180);"
        "border: 2px solid white;"
        "border-radius: 20px;"
        "font-size: 22px;"
        "font-weight: bold;"
        "font-family: 'OCR A Extended';"
        "padding: 20px;"
        "}");

    Client_Button->setStyleSheet(
        "QPushButton {"
        "background-color: rgba(120, 180, 255, 180);"
        "border: 2px solid white;"
        "border-radius: 20px;"
        "font-size: 19px;"
        "font-weight: bold;"
        "font-family: 'OCR A Extended';"
        "padding: 20px;"
        "}");

    Back_Button->setStyleSheet(
        "QPushButton {"
        "background-color: rgba(40, 40, 40, 220);"
        "color: white;"
        "border: 2px solid white;"
        "border-radius: 20px;"
        "font-size: 19px;"
        "font-weight: bold;"
        "font-family: 'OCR A Extended';"
        "padding: 20px;"
        "}");

    QGridLayout *Layout = new QGridLayout(this);

    //pull da buttons in a lil so they not stuck to da edges
    Layout->setContentsMargins(60, 45, 60, 45);
    Layout->setHorizontalSpacing(35);
    Layout->setVerticalSpacing(35);

    //make da grid balanced so da diagonal looks proper
    Layout->setRowStretch(0, 1);
    Layout->setRowStretch(1, 1);
    Layout->setRowStretch(2, 1);
    Layout->setColumnStretch(0, 1);
    Layout->setColumnStretch(1, 1);
    Layout->setColumnStretch(2, 1);

    //keep da diagonal layout
    Layout->addWidget(Host_Button, 0, 0, Qt::AlignTop | Qt::AlignLeft);
    Layout->addWidget(Back_Button, 1, 1, Qt::AlignCenter);
    Layout->addWidget(Client_Button, 2, 2, Qt::AlignBottom | Qt::AlignRight);

    setLayout(Layout);
    Character_Label->raise();
}

void Menu_Multiplayer_Screen::Update_Frame()
{
    if (Frames.isEmpty())
        return;
    QPixmap scaled = Frames[Current_Frame].scaled(
            Character_Label->size(),
            Qt::KeepAspectRatio,
            Qt::FastTransformation
        );

    Character_Label->setPixmap(scaled);

    Current_Frame = (Current_Frame + 1) % Frames.size();
    qDebug() << "Update_Frame called, current frame =" << Current_Frame;
}

void Menu_Multiplayer_Screen::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    Character_Label->setFixedSize(384,384);
    Character_Label->move(30, height() - Character_Label->height() - 30);
    Character_Label->raise();
    Character_Label->show();

}

QPushButton *Menu_Multiplayer_Screen::Get_Host_Button() const { return Host_Button; }
QPushButton *Menu_Multiplayer_Screen::Get_Client_Button() const { return Client_Button; }
QPushButton *Menu_Multiplayer_Screen::Get_Back_Button() const { return Back_Button; }
