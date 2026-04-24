#include "menu_multiplayer_screen.h"

#include <QGridLayout>
#include <QPushButton>
#include <QSizePolicy>
#include <QFont>
#include <QHideEvent>
#include <QPixmap>
#include <QDebug>
#include <QResizeEvent>
#include <QShowEvent>
#include <QTimer>

Menu_Multiplayer_Screen::Menu_Multiplayer_Screen(QWidget *parent)
    : QWidget(parent)
    , Current_Frame(0)
    , Completed_Animation_Loops(0)
    , Animation_Status_Reported(false)
{
    setAttribute(Qt::WA_StyledBackground, true);
    QPixmap test(":/Images/Background/Space_BG_2.png");
    qDebug() << test.isNull();

    // multiplayer screen background
    setObjectName("MultiplayerMenu");
    setStyleSheet(
        "#MultiplayerMenu {"
        "border-image: url(:/Images/Background/Space_BG_2.png) 0 0 0 0 stretch stretch;"
        "}"
    );

    // load the sprite sheet and cut it into frames
    Sprite_Sheet = new QPixmap(":/Images/Characters/Sitting_Black.png");
    qDebug() << "sprite null?" << Sprite_Sheet->isNull();
    qDebug() << "sprite size:" << Sprite_Sheet->width() << Sprite_Sheet->height();

    int Frame_Width = Sprite_Sheet->width() / 8;
    int Frame_Height = Sprite_Sheet->height();

    Frames.clear();
    for (int x = 0; x + Frame_Width <= Sprite_Sheet->width(); x += Frame_Width)
    {
        Frames.append(Sprite_Sheet->copy(x, 0, Frame_Width, Frame_Height));
    }
    qDebug() << "frames count:" << Frames.size();

    // animated character above the layout
    Character_Label = new QLabel(this);
    Character_Label->setAttribute(Qt::WA_TranslucentBackground);
    Character_Label->raise();

    if (!Frames.isEmpty())
    {
        Character_Label->setPixmap(Frames[0]);
        qDebug() << "first frame size:" << Frames[0].size();
    }

    // timer for the idle animation
    Multiplayer_Character_Timer = new QTimer(this);
    connect(Multiplayer_Character_Timer, &QTimer::timeout,this, &Menu_Multiplayer_Screen::Update_Frame);

    // main buttons for this screen
    Host_Button = new QPushButton("Become The Last\nConstellation", this);
    Client_Button = new QPushButton("Traverse The Stars", this);
    Back_Button = new QPushButton("Return To The Void", this);

    // keep button sizes consistent
    Host_Button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    Client_Button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    Back_Button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

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

    // lighter style for host and client, darker style for back
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

    // grid layout for the diagonal button placement
    QGridLayout *Layout = new QGridLayout(this);

    // pull the buttons away from the edges
    Layout->setContentsMargins(60, 45, 60, 45);
    Layout->setHorizontalSpacing(35);
    Layout->setVerticalSpacing(35);

    // balance the grid for the diagonal layout
    Layout->setRowStretch(0, 1);
    Layout->setRowStretch(1, 1);
    Layout->setRowStretch(2, 1);
    Layout->setColumnStretch(0, 1);
    Layout->setColumnStretch(1, 1);
    Layout->setColumnStretch(2, 1);

    // keep the diagonal button flow
    Layout->addWidget(Host_Button, 0, 0, Qt::AlignTop | Qt::AlignLeft);
    Layout->addWidget(Back_Button, 1, 1, Qt::AlignCenter);
    Layout->addWidget(Client_Button, 2, 2, Qt::AlignBottom | Qt::AlignRight);

    setLayout(Layout);
    Character_Label->raise();

    Apply_Responsive_Sizing();
}

void Menu_Multiplayer_Screen::Update_Frame()
{
    if (Frames.isEmpty())
    {
        Report_Animation_Error_Once();
        return;
    }

    // scale the current frame before advancing
    QPixmap scaled = Frames[Current_Frame].scaled(
            Character_Label->size(),
            Qt::KeepAspectRatio,
            Qt::FastTransformation
        );

    Character_Label->setPixmap(scaled);

    Current_Frame = (Current_Frame + 1) % Frames.size();

    if(Current_Frame == 0)
    {
        Completed_Animation_Loops++;

        if(Completed_Animation_Loops >= 3 && !Animation_Status_Reported)
        {
            Animation_Status_Reported = true;
            qDebug() << "Animation successful: Multiplayer menu";
        }
    }
}

void Menu_Multiplayer_Screen::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    Apply_Responsive_Sizing();
}

void Menu_Multiplayer_Screen::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);

    if(Multiplayer_Character_Timer != nullptr && !Multiplayer_Character_Timer->isActive())
    {
        Multiplayer_Character_Timer->start(150);
        qDebug() << "Animation started: Multiplayer menu";
    }
}

void Menu_Multiplayer_Screen::hideEvent(QHideEvent *event)
{
    QWidget::hideEvent(event);

    if(Multiplayer_Character_Timer != nullptr && Multiplayer_Character_Timer->isActive())
    {
        Multiplayer_Character_Timer->stop();
        qDebug() << "Animation stopped: Multiplayer menu";
    }
}

void Menu_Multiplayer_Screen::Apply_Responsive_Sizing()
{
    const int w = width();
    const int h = height();

    const int Button_Width = qMax(160, (260 * w) / 1280);
    const int Button_Height = qMax(72, (120 * h) / 720);

    Host_Button->setMinimumSize(Button_Width, Button_Height);
    Host_Button->setMaximumSize(Button_Width, Button_Height);
    Client_Button->setMinimumSize(Button_Width, Button_Height);
    Client_Button->setMaximumSize(Button_Width, Button_Height);
    Back_Button->setMinimumSize(Button_Width, Button_Height);
    Back_Button->setMaximumSize(Button_Width, Button_Height);

    QGridLayout *Grid = qobject_cast<QGridLayout*>(layout());
    if(Grid != nullptr)
    {
        Grid->setContentsMargins(qMax(20, (60 * w) / 1280),
                                 qMax(16, (45 * h) / 720),
                                 qMax(20, (60 * w) / 1280),
                                 qMax(16, (45 * h) / 720));
        Grid->setHorizontalSpacing(qMax(12, (35 * w) / 1280));
        Grid->setVerticalSpacing(qMax(12, (35 * h) / 720));
    }

    if(Character_Label != nullptr)
    {
        Character_Label->resize(qMax(96, (384 * w) / 1280),
                                qMax(96, (384 * h) / 720));
        Character_Label->move(qMax(12, (30 * w) / 1280),
                              height() - Character_Label->height() - qMax(12, (30 * h) / 720));
        Character_Label->raise();
        Character_Label->show();
    }
}

void Menu_Multiplayer_Screen::Report_Animation_Error_Once()
{
    if(Animation_Status_Reported)
        return;

    Animation_Status_Reported = true;
    qDebug() << "Animation error: Multiplayer menu";
}

QPushButton *Menu_Multiplayer_Screen::Get_Host_Button() const { return Host_Button; }
QPushButton *Menu_Multiplayer_Screen::Get_Client_Button() const { return Client_Button; }
QPushButton *Menu_Multiplayer_Screen::Get_Back_Button() const { return Back_Button; }
