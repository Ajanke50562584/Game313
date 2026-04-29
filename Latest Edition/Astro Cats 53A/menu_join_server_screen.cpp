#include "menu_join_server_screen.h"

#include <QFont>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QResizeEvent>
#include <QVBoxLayout>
#include <QWidget>

Menu_Join_Server_Screen::Menu_Join_Server_Screen(QWidget *parent)
    : QWidget(parent)
{
    // outer layout for the join screen
    Main_Layout = new QVBoxLayout(this);
    Main_Layout->setContentsMargins(24, 24, 24, 24);
    Main_Layout->setSpacing(16);
    setAttribute(Qt::WA_StyledBackground, true);

    setObjectName("ServerMenu");
    setStyleSheet(
        "#ServerMenu {"
        "border-image: url(:/Images/Background/Space_BG_3.png) 0 0 0 0 stretch stretch;"
        "}"
    );

    // screen heading
    Title_Label = new QLabel("Traverse The Stars", this);
    Title_Label->setAlignment(Qt::AlignCenter);
    Title_Label->setStyleSheet(
                "color: rgb(235,235,245);"
                "background-color: rgba(0, 74, 113, 237);"
                "font-weight: 700;"
                "font-weight: bold;"
                "font-family: 'OCR A Extended';");

    Sub_Title_Label = new QLabel("Find public lobbies drifting through the cosmos.", this);
    Sub_Title_Label->setAlignment(Qt::AlignCenter);
    Sub_Title_Label->setStyleSheet(
                "color: rgb(230,235,245);"
                "font-family: 'OCR A Extended';"
                "background: transparent;");

    // main card that holds the join form and lobby list
    Card_Widget = new QWidget(this);
    Card_Widget->setStyleSheet(
                "background-color: rgba(255,255,255,145);"
                "border: 2px solid rgba(245,245,255,170);");

    Card_Layout = new QVBoxLayout(Card_Widget);
    Card_Layout->setContentsMargins(26, 26, 26, 26);
    Card_Layout->setSpacing(16);

    Player_Name_Label = new QLabel("Player Name", Card_Widget);
    Player_Name_Label->setStyleSheet(
                "color: rgb(24,28,38);"
                "font-weight: 600;"
                "font-family: 'OCR A Extended';"
                "background: transparent;");

    Player_Name_Input = new QLineEdit(Card_Widget);
    Player_Name_Input->setPlaceholderText("Player");

    // keep names short
    Player_Name_Validator = new QRegularExpressionValidator(
                QRegularExpression("^[A-Za-z0-9 _\\-]{0,24}$"),
                Player_Name_Input);
    Player_Name_Input->setValidator(Player_Name_Validator);
    Player_Name_Input->setStyleSheet(
                "QLineEdit {"
                "font-family: 'OCR A Extended';"
                "background-color: rgba(255,255,255,215);"
                "border: 1px solid rgba(80,90,120,120);"
                "color: rgb(15,20,30);"
                "}");

    Lobby_List = new QListWidget(Card_Widget);
    Lobby_List->setStyleSheet(
                "QListWidget {"
                "font-family: 'OCR A Extended';"
                "background-color: rgba(255,255,255,215);"
                "border: 1px solid rgba(80,90,120,120);"
                "selection-background-color: rgba(120,180,255,180);"
                "color: rgb(15,20,30);"
                "}");

    Join_Button = new QPushButton("Join Lobby", this);
    Refresh_Button = new QPushButton("Refresh", this);
    Back_Button = new QPushButton("Back", this);

    Join_Button->setCursor(Qt::PointingHandCursor);
    Refresh_Button->setCursor(Qt::PointingHandCursor);
    Back_Button->setCursor(Qt::PointingHandCursor);

    connect(Join_Button, SIGNAL(pressed()), this, SLOT(Set_Button_Cursor_To_Closed_Hand()));
    connect(Join_Button, SIGNAL(released()), this, SLOT(Set_Button_Cursor_To_Pointing_Hand()));
    connect(Refresh_Button, SIGNAL(pressed()), this, SLOT(Set_Button_Cursor_To_Closed_Hand()));
    connect(Refresh_Button, SIGNAL(released()), this, SLOT(Set_Button_Cursor_To_Pointing_Hand()));
    connect(Back_Button, SIGNAL(pressed()), this, SLOT(Set_Button_Cursor_To_Closed_Hand()));
    connect(Back_Button, SIGNAL(released()), this, SLOT(Set_Button_Cursor_To_Pointing_Hand()));

    // add inputs first, then the lobby list, then actions
    Card_Layout->addWidget(Player_Name_Label);
    Card_Layout->addWidget(Player_Name_Input);
    Card_Layout->addWidget(Lobby_List);

    QHBoxLayout *Button_Row = new QHBoxLayout();
    Button_Row->setSpacing(14);
    Button_Row->addWidget(Join_Button);
    Button_Row->addWidget(Refresh_Button);
    Button_Row->addWidget(Back_Button);

    Card_Layout->addLayout(Button_Row);

    // helper rows keep the layout centered
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

    Apply_Responsive_Styles();
}

QPushButton *Menu_Join_Server_Screen::Get_Join_Button() const { return Join_Button; }
QPushButton *Menu_Join_Server_Screen::Get_Refresh_Button() const { return Refresh_Button; }
QPushButton *Menu_Join_Server_Screen::Get_Back_Button() const { return Back_Button; }
QListWidget *Menu_Join_Server_Screen::Get_Lobby_List() const { return Lobby_List; }
QLineEdit *Menu_Join_Server_Screen::Get_Player_Name_Input() const { return Player_Name_Input; }

void Menu_Join_Server_Screen::resizeEvent(QResizeEvent *event)
{
    const int w = width();
    const int h = height();

    Main_Layout->setContentsMargins(qMax(12, (24 * w) / 1280),
                                    qMax(12, (24 * h) / 720),
                                    qMax(12, (24 * w) / 1280),
                                    qMax(12, (24 * h) / 720));
    Main_Layout->setSpacing(qMax(8, (16 * h) / 720));

    Card_Widget->setMaximumWidth(qMax(320, (760 * w) / 1280));
    Card_Layout->setContentsMargins(qMax(14, (26 * w) / 1280),
                                    qMax(14, (26 * h) / 720),
                                    qMax(14, (26 * w) / 1280),
                                    qMax(14, (26 * h) / 720));
    Card_Layout->setSpacing(qMax(8, (16 * h) / 720));
    Lobby_List->setMinimumHeight(qMax(160, (280 * h) / 720));

    // resize the join screen text with the window
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

    const int Button_Height = qMax(40, (58 * h) / 720);
    Join_Button->setMinimumHeight(Button_Height);
    Refresh_Button->setMinimumHeight(Button_Height);
    Back_Button->setMinimumHeight(Button_Height);

    Apply_Responsive_Styles();

    QWidget::resizeEvent(event);
}

void Menu_Join_Server_Screen::Apply_Responsive_Styles()
{
    const int w = qMax(width(), 640);
    const int h = qMax(height(), 360);
    const int Title_Radius = qMax(12, (18 * h) / 720);
    const int Title_Pad_V = qMax(7, (12 * h) / 720);
    const int Title_Pad_H = qMax(12, (22 * w) / 1280);
    const int Card_Radius = qMax(14, (22 * h) / 720);
    const int Field_Radius = qMax(10, (16 * h) / 720);
    const int Field_Pad_V = qMax(6, (12 * h) / 720);
    const int Field_Pad_H = qMax(10, (16 * w) / 1280);
    const int List_Pad = qMax(5, (8 * h) / 720);
    const int Button_Radius = qMax(14, (24 * h) / 720);
    const int Button_Pad_V = qMax(7, (12 * h) / 720);
    const int Button_Pad_H = qMax(12, (24 * w) / 1280);

    Title_Label->setStyleSheet(QString(
        "color: rgb(235,235,245);"
        "background-color: rgba(0, 74, 113, 237);"
        "border-radius: %1px;"
        "padding: %2px %3px;"
        "font-weight: 700;"
        "font-family: 'OCR A Extended';")
        .arg(Title_Radius).arg(Title_Pad_V).arg(Title_Pad_H));

    Card_Widget->setStyleSheet(QString(
        "background-color: rgba(255,255,255,145);"
        "border: 2px solid rgba(245,245,255,170);"
        "border-radius: %1px;").arg(Card_Radius));

    Player_Name_Input->setStyleSheet(QString(
        "QLineEdit {"
        "font-family: 'OCR A Extended';"
        "background-color: rgba(255,255,255,215);"
        "border: 1px solid rgba(80,90,120,120);"
        "border-radius: %1px;"
        "padding: %2px %3px;"
        "color: rgb(15,20,30);"
        "}").arg(Field_Radius).arg(Field_Pad_V).arg(Field_Pad_H));

    Lobby_List->setStyleSheet(QString(
        "QListWidget {"
        "font-family: 'OCR A Extended';"
        "background-color: rgba(255,255,255,215);"
        "border: 1px solid rgba(80,90,120,120);"
        "border-radius: %1px;"
        "padding: %2px;"
        "selection-background-color: rgba(120,180,255,180);"
        "color: rgb(15,20,30);"
        "}").arg(Field_Radius).arg(List_Pad));

    const QString Button_Style = QString(
        "QPushButton {"
        "background-color: rgb(120, 180, 255);"
        "color: rgb(15,20,30);"
        "border: none;"
        "border-radius: %1px;"
        "padding: %2px %3px;"
        "font-weight: 600;"
        "font-family: 'OCR A Extended';"
        "}"
        "QPushButton:hover {"
        "background-color: rgb(140, 200, 255);"
        "}"
        "QPushButton:pressed {"
        "background-color: rgb(100,160,245);"
        "}").arg(Button_Radius).arg(Button_Pad_V).arg(Button_Pad_H);

    Join_Button->setStyleSheet(Button_Style);
    Refresh_Button->setStyleSheet(Button_Style);
    Back_Button->setStyleSheet(Button_Style);
}

void Menu_Join_Server_Screen::Set_Button_Cursor_To_Closed_Hand()
{
    QPushButton *Button = qobject_cast<QPushButton*>(sender());

    if(Button != nullptr)
        Button->setCursor(Qt::ClosedHandCursor);
}

void Menu_Join_Server_Screen::Set_Button_Cursor_To_Pointing_Hand()
{
    QPushButton *Button = qobject_cast<QPushButton*>(sender());

    if(Button != nullptr)
        Button->setCursor(Qt::PointingHandCursor);
}
