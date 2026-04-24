#include "menu_create_lobby_screen.h"

#include <QCheckBox>
#include <QFont>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QResizeEvent>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>

Menu_Create_Lobby_Screen::Menu_Create_Lobby_Screen(QWidget *parent)
    : QWidget(parent)
{
    // outer layout for the full create lobby screen
    Main_Layout = new QVBoxLayout(this);
    Main_Layout->setContentsMargins(24, 24, 24, 24);
    Main_Layout->setSpacing(16);
    setAttribute(Qt::WA_StyledBackground, true);

    setObjectName("ServerMenu");
    setStyleSheet(
        "#ServerMenu {"
        "border-image: url(:/Images/Background/Space_BG_4) 0 0 0 0 stretch stretch;"
        "}"
    );


    // top title + subtitle
    Title_Label = new QLabel("Build Your Constellation", this);
    Title_Label->setAlignment(Qt::AlignCenter);
    Title_Label->setStyleSheet(
                "color: rgb(235,235,245);"
                "background-color: rgba(0, 74, 113, 237);"
                "border-radius: 18px;"
                "padding: 12px 22px;"
                "font-weight: 700;"
                "font-weight: bold;"
                "font-family: 'OCR A Extended';");

    Sub_Title_Label = new QLabel("Create a new lobby for your crew.", this);
    Sub_Title_Label->setAlignment(Qt::AlignCenter);
    Sub_Title_Label->setStyleSheet(
                "color: rgb(230,235,245);"
                "font-family: 'OCR A Extended';"
                "background: transparent;");

    // centered glass card for the host form
    Card_Widget = new QWidget(this);
    Card_Widget->setStyleSheet(
                "background-color: rgba(255,255,255,145);"
                "border: 2px solid rgba(245,245,255,170);"
                "border-radius: 22px;");

    Card_Layout = new QVBoxLayout(Card_Widget);
    Card_Layout->setContentsMargins(26, 26, 26, 26);
    Card_Layout->setSpacing(16);

    Lobby_Name_Label = new QLabel("Lobby Name", Card_Widget);
    Max_Players_Label = new QLabel("Max Players", Card_Widget);
    Password_Label = new QLabel("Set Password", Card_Widget);

    // shared label styling for the form
    const QString labelStyle =
            "color: rgb(24,28,38);"
            "font-weight: 600;"
            "font-family: 'OCR A Extended';"
            "background: transparent;";

    Lobby_Name_Label->setStyleSheet(labelStyle);
    Max_Players_Label->setStyleSheet(labelStyle);
    Password_Label->setStyleSheet(labelStyle);

    Lobby_Name_Input = new QLineEdit(Card_Widget);
    Lobby_Name_Input->setPlaceholderText("The Last Constellation Lobby");
    Lobby_Name_Input->setText("The Last Constellation Lobby");

    // allow concise lobby names up to 32 characters
    Lobby_Name_Validator = new QRegularExpressionValidator(
                QRegularExpression("^[A-Za-z0-9 _\\-]{0,32}$"),
                Lobby_Name_Input);
    Lobby_Name_Input->setValidator(Lobby_Name_Validator);
    Lobby_Name_Input->setStyleSheet(
                "QLineEdit {"
                "font-family: 'OCR A Extended';"
                "background-color: rgba(255,255,255,215);"
                "border: 1px solid rgba(80,90,120,120);"
                "border-radius: 16px;"
                "padding: 12px 16px;"
                "color: rgb(15,20,30);"
                "}");

    Max_Players_Input = new QSpinBox(Card_Widget);
    Max_Players_Input->setRange(2, 4);
    Max_Players_Input->setValue(4);
    Max_Players_Input->setStyleSheet(
                "QSpinBox {"
                "background-color: rgba(255,255,255,215);"
                "font-weight: bold;"
                "font-family: 'OCR A Extended';"
                "border: 1px solid rgba(80,90,120,120);"
                "border-radius: 16px;"
                "padding: 10px 14px;"
                "color: rgb(15,20,30);"
                "}");

    Public_Lobby_Checkbox = new QCheckBox("Public Lobby", Card_Widget);
    Public_Lobby_Checkbox->setChecked(true);
    Public_Lobby_Checkbox->setStyleSheet(
                "QCheckBox {"
                "color: rgb(24,28,38);"
                "font-weight: 600;"
                "font-family: 'OCR A Extended';"
                "spacing: 10px;"
                "}");

    // add the password field for private lobby setup
    Password_Input = new QLineEdit(Card_Widget);
    Password_Input->setPlaceholderText("Enter a password if the lobby is private");
    Password_Input->setEchoMode(QLineEdit::Password);
    Password_Input->setEnabled(false);
    Password_Input->setStyleSheet(
                "QLineEdit {"
                "font-family: 'OCR A Extended';"
                "background-color: rgba(255,255,255,215);"
                "border: 1px solid rgba(80,90,120,120);"
                "border-radius: 16px;"
                "padding: 12px 16px;"
                "color: rgb(15,20,30);"
                "}"
                "QLineEdit:disabled {"
                "background-color: rgba(220,220,220,180);"
                "color: rgba(70,70,70,180);"
                "}");

    Show_Password_Checkbox = new QCheckBox("Show Password", Card_Widget);
    Show_Password_Checkbox->setEnabled(false);
    Show_Password_Checkbox->setStyleSheet(
                "QCheckBox {"
                "color: rgb(24,28,38);"
                "font-weight: 600;"
                "font-family: 'OCR A Extended';"
                "spacing: 10px;"
                "}");

    Create_Lobby_Button = new QPushButton("Create Lobby", Card_Widget);
    Back_Button = new QPushButton("Back", Card_Widget);

    // one shared look for the action buttons
    const QString buttonStyle =
            "QPushButton {"
            "background-color: rgb(160,165,175);"
            "color: rgb(15,20,30);"
            "border: none;"
            "border-radius: 24px;"
            "padding: 12px 24px;"
            "font-weight: 600;"
            "font-family: 'OCR A Extended';"
            "}"
            "QPushButton:hover {"
            "background-color: rgb(180,185,195);"
            "}"
            "QPushButton:pressed {"
            "background-color: rgb(140,145,155);"
            "}";

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

    // only use the password controls when the lobby is private
    connect(Public_Lobby_Checkbox, &QCheckBox::toggled, this, [=](bool checked) {
        const bool isPrivate = !checked;
        Password_Input->setEnabled(isPrivate);
        Show_Password_Checkbox->setEnabled(isPrivate);

        if(!isPrivate)
        {
            Password_Input->clear();
            Password_Input->setEchoMode(QLineEdit::Password);
            Show_Password_Checkbox->setChecked(false);
        }
    });

    connect(Show_Password_Checkbox, &QCheckBox::toggled, this, [=](bool checked) {
        Password_Input->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
    });

    // stack the form fields inside the card
    Card_Layout->addWidget(Lobby_Name_Label);
    Card_Layout->addWidget(Lobby_Name_Input);
    Card_Layout->addWidget(Max_Players_Label);
    Card_Layout->addWidget(Max_Players_Input);
    Card_Layout->addWidget(Public_Lobby_Checkbox);
    Card_Layout->addWidget(Password_Label);
    Card_Layout->addWidget(Password_Input);
    Card_Layout->addWidget(Show_Password_Checkbox);

    QHBoxLayout *Button_Row = new QHBoxLayout();
    Button_Row->setSpacing(14);
    Button_Row->addWidget(Create_Lobby_Button);
    Button_Row->addWidget(Back_Button);
    Card_Layout->addLayout(Button_Row);

    // helper rows keep the title and card centered on wide screens
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

QPushButton *Menu_Create_Lobby_Screen::Get_Create_Lobby_Button() const { return Create_Lobby_Button; }
QPushButton *Menu_Create_Lobby_Screen::Get_Back_Button() const { return Back_Button; }
QLineEdit *Menu_Create_Lobby_Screen::Get_Lobby_Name_Input() const { return Lobby_Name_Input; }
QSpinBox *Menu_Create_Lobby_Screen::Get_Max_Players_Input() const { return Max_Players_Input; }
QCheckBox *Menu_Create_Lobby_Screen::Get_Public_Lobby_Checkbox() const { return Public_Lobby_Checkbox; }
QLineEdit *Menu_Create_Lobby_Screen::Get_Password_Input() const { return Password_Input; }

void Menu_Create_Lobby_Screen::resizeEvent(QResizeEvent *event)
{
    const int w = width();
    const int h = height();

    Main_Layout->setContentsMargins(qMax(12, (24 * w) / 1280),
                                    qMax(12, (24 * h) / 720),
                                    qMax(12, (24 * w) / 1280),
                                    qMax(12, (24 * h) / 720));
    Main_Layout->setSpacing(qMax(8, (16 * h) / 720));

    Card_Widget->setMaximumWidth(qMax(300, (620 * w) / 1280));
    Card_Layout->setContentsMargins(qMax(14, (26 * w) / 1280),
                                    qMax(14, (26 * h) / 720),
                                    qMax(14, (26 * w) / 1280),
                                    qMax(14, (26 * h) / 720));
    Card_Layout->setSpacing(qMax(8, (16 * h) / 720));

    // scale text down and up with the screen width
    QFont Title_Font;
    Title_Font.setPointSize(qMax(18, w / 38));
    Title_Label->setFont(Title_Font);

    QFont Sub_Font;
    Sub_Font.setPointSize(qMax(10, w / 82));
    Sub_Title_Label->setFont(Sub_Font);

    QFont Input_Font;
    Input_Font.setPointSize(qMax(10, w / 88));
    Lobby_Name_Label->setFont(Input_Font);
    Max_Players_Label->setFont(Input_Font);
    Password_Label->setFont(Input_Font);
    Lobby_Name_Input->setFont(Input_Font);
    Max_Players_Input->setFont(Input_Font);
    Public_Lobby_Checkbox->setFont(Input_Font);
    Password_Input->setFont(Input_Font);
    Show_Password_Checkbox->setFont(Input_Font);
    Create_Lobby_Button->setFont(Input_Font);
    Back_Button->setFont(Input_Font);

    const int Button_Height = qMax(40, (58 * h) / 720);
    Create_Lobby_Button->setMinimumHeight(Button_Height);
    Back_Button->setMinimumHeight(Button_Height);

    QWidget::resizeEvent(event);
}
