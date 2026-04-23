#include "menu_multiplayer_screen.h"

#include <QGridLayout>
#include <QPushButton>
#include <QSizePolicy>

Menu_Multiplayer_Screen::Menu_Multiplayer_Screen(QWidget *parent)
    : QWidget(parent)
{
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
}

QPushButton *Menu_Multiplayer_Screen::Get_Host_Button() const { return Host_Button; }
QPushButton *Menu_Multiplayer_Screen::Get_Client_Button() const { return Client_Button; }
QPushButton *Menu_Multiplayer_Screen::Get_Back_Button() const { return Back_Button; }
