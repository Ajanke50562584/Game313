
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
    // outer layout for the create lobby screen
    Main_Layout = new QVBoxLayout(this);
    Main_Layout->setContentsMargins(24, 24, 24, 24);
    Main_Layout->setSpacing(16);
    setAttribute(Qt::WA_StyledBackground, true);

    setObjectName("ServerMenu");
    setStyleSheet(
        "#ServerMenu {"
        "border-image: url(:/Images/Background/Space_BG_4.png) 0 0 0 0 stretch stretch;"
        "}"
    );


    // top title and subtitle
    Title_Label = new QLabel("Build Your Constellation", this);
    Title_Label->setAlignment(Qt::AlignCenter);
    Title_Label->setStyleSheet(
                "color: rgb(235,235,245);"
                "background-color: rgba(0, 74, 113, 237);"
                "font-weight: 700;"
                "font-weight: bold;"
                "font-family: 'OCR A Extended';");

    Sub_Title_Label = new QLabel("Create a new lobby for your crew.", this);
    Sub_Title_Label->setAlignment(Qt::AlignCenter);
    Sub_Title_Label->setStyleSheet(
                "color: rgb(230,235,245);"
                "font-family: 'OCR A Extended';"
                "background: transparent;");

    // centered card for the host form
    Card_Widget = new QWidget(this);
    Card_Widget->setStyleSheet(
                "background-color: rgba(255,255,255,145);"
                "border: 2px solid rgba(245,245,255,170);");

    Card_Layout = new QVBoxLayout(Card_Widget);
    Card_Layout->setContentsMargins(26, 26, 26, 26);
    Card_Layout->setSpacing(16);

    Lobby_Name_Label = new QLabel("Lobby Name", Card_Widget);
    Host_Name_Label = new QLabel("Host Name", Card_Widget);
    Max_Players_Label = new QLabel("Max Players", Card_Widget);
    Password_Label = new QLabel("Set Password", Card_Widget);

    // shared label style
    const QString labelStyle =
            "color: rgb(24,28,38);"
            "font-weight: 600;"
            "font-family: 'OCR A Extended';"
            "background: transparent;";

    Lobby_Name_Label->setStyleSheet(labelStyle);
    Host_Name_Label->setStyleSheet(labelStyle);
    Max_Players_Label->setStyleSheet(labelStyle);
    Password_Label->setStyleSheet(labelStyle);

    Lobby_Name_Input = new QLineEdit(Card_Widget);
    Lobby_Name_Input->setPlaceholderText("The Last Constellation Lobby");
    Lobby_Name_Input->setText("The Last Constellation Lobby");

    // allow short lobby names
    Lobby_Name_Validator = new QRegularExpressionValidator(
                QRegularExpression("^[A-Za-z0-9 _\\-]{0,32}$"),
                Lobby_Name_Input);
    // only allow uppercase letters A-Z
    // only allow lowercase letters a-z
    // only allow numbers 0-9
    // also allow a space
    // also allow _
    // also allow -
    // length must be from 0 to 32 characters

    Lobby_Name_Input->setValidator(Lobby_Name_Validator);
    Lobby_Name_Input->setStyleSheet(
                "QLineEdit {"
                "font-family: 'OCR A Extended';"
                "background-color: rgba(255,255,255,215);"
                "border: 1px solid rgba(80,90,120,120);"
                "color: rgb(15,20,30);"
                "}");

    Host_Name_Input = new QLineEdit(Card_Widget);
    Host_Name_Input->setPlaceholderText("Enter host name");

    Host_Name_Validator = new QRegularExpressionValidator(
                QRegularExpression("^[A-Za-z0-9 _\\-]{0,24}$"),
                Host_Name_Input);

    Host_Name_Input->setValidator(Host_Name_Validator);
    Host_Name_Input->setStyleSheet(
                "QLineEdit {"
                "font-family: 'OCR A Extended';"
                "background-color: rgba(255,255,255,215);"
                "border: 1px solid rgba(80,90,120,120);"
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

    // add the password field
    Password_Input = new QLineEdit(Card_Widget);
    Password_Input->setPlaceholderText("Enter a password if the lobby is private");
    Password_Input->setEchoMode(QLineEdit::Password);
    Password_Input->setEnabled(false);
    Password_Input->setStyleSheet(
                "QLineEdit {"
                "font-family: 'OCR A Extended';"
                "background-color: rgba(255,255,255,215);"
                "border: 1px solid rgba(80,90,120,120);"
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

    Create_Lobby_Button->setCursor(Qt::PointingHandCursor);
    Back_Button->setCursor(Qt::PointingHandCursor);

    connect(Create_Lobby_Button, SIGNAL(pressed()), this, SLOT(Set_Button_Cursor_To_Closed_Hand()));
    connect(Create_Lobby_Button, SIGNAL(released()), this, SLOT(Set_Button_Cursor_To_Pointing_Hand()));
    connect(Back_Button, SIGNAL(pressed()), this, SLOT(Set_Button_Cursor_To_Closed_Hand()));
    connect(Back_Button, SIGNAL(released()), this, SLOT(Set_Button_Cursor_To_Pointing_Hand()));

    // only use the password controls when the lobby is private
    connect(Public_Lobby_Checkbox, SIGNAL(toggled(bool)), this, SLOT(Handle_Public_Lobby_Toggled(bool)));
    connect(Show_Password_Checkbox, SIGNAL(toggled(bool)), this, SLOT(Handle_Show_Password_Toggled(bool)));
    connect(Host_Name_Input, &QLineEdit::textChanged, this, &Menu_Create_Lobby_Screen::Handle_Host_Name_Changed);

    // stack the form fields inside the card
    Card_Layout->addWidget(Lobby_Name_Label);
    Card_Layout->addWidget(Lobby_Name_Input);
    Card_Layout->addWidget(Host_Name_Label);
    Card_Layout->addWidget(Host_Name_Input);
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

    // helper rows keep the title and card centered
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

    Update_Create_Button_State();
    Apply_Responsive_Styles();
}

QPushButton *Menu_Create_Lobby_Screen::Get_Create_Lobby_Button() const { return Create_Lobby_Button; }
QPushButton *Menu_Create_Lobby_Screen::Get_Back_Button() const { return Back_Button; }
QLineEdit *Menu_Create_Lobby_Screen::Get_Lobby_Name_Input() const { return Lobby_Name_Input; }
QLineEdit *Menu_Create_Lobby_Screen::Get_Host_Name_Input() const { return Host_Name_Input; }
QSpinBox *Menu_Create_Lobby_Screen::Get_Max_Players_Input() const { return Max_Players_Input; }
QCheckBox *Menu_Create_Lobby_Screen::Get_Public_Lobby_Checkbox() const { return Public_Lobby_Checkbox; }

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

    // scale text with the screen width
    QFont Title_Font;
    Title_Font.setPointSize(qMax(18, w / 38));
    Title_Label->setFont(Title_Font);

    QFont Sub_Font;
    Sub_Font.setPointSize(qMax(10, w / 82));
    Sub_Title_Label->setFont(Sub_Font);

    QFont Input_Font;
    Input_Font.setPointSize(qMax(10, w / 88));
    Lobby_Name_Label->setFont(Input_Font);
    Host_Name_Label->setFont(Input_Font);
    Max_Players_Label->setFont(Input_Font);
    Password_Label->setFont(Input_Font);
    Lobby_Name_Input->setFont(Input_Font);
    Host_Name_Input->setFont(Input_Font);
    Max_Players_Input->setFont(Input_Font);
    Public_Lobby_Checkbox->setFont(Input_Font);
    Password_Input->setFont(Input_Font);
    Show_Password_Checkbox->setFont(Input_Font);
    Create_Lobby_Button->setFont(Input_Font);
    Back_Button->setFont(Input_Font);

    const int Button_Height = qMax(40, (58 * h) / 720);
    Create_Lobby_Button->setMinimumHeight(Button_Height);
    Back_Button->setMinimumHeight(Button_Height);

    Apply_Responsive_Styles();

    QWidget::resizeEvent(event);
}

void Menu_Create_Lobby_Screen::Apply_Responsive_Styles()
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
    const int Small_Field_Pad_V = qMax(5, (10 * h) / 720);
    const int Small_Field_Pad_H = qMax(9, (14 * w) / 1280);
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

    Lobby_Name_Input->setStyleSheet(QString(
        "QLineEdit {"
        "font-family: 'OCR A Extended';"
        "background-color: rgba(255,255,255,215);"
        "border: 1px solid rgba(80,90,120,120);"
        "border-radius: %1px;"
        "padding: %2px %3px;"
        "color: rgb(15,20,30);"
        "}").arg(Field_Radius).arg(Field_Pad_V).arg(Field_Pad_H));

    const bool Has_Host_Name = !Host_Name_Input->text().trimmed().isEmpty();
    Host_Name_Input->setStyleSheet(QString(
        "QLineEdit {"
        "font-family: 'OCR A Extended';"
        "background-color: rgba(255,255,255,215);"
        "border: 2px solid %1;"
        "border-radius: %2px;"
        "padding: %3px %4px;"
        "color: rgb(15,20,30);"
        "}").arg(Has_Host_Name ? "rgba(80,90,120,120)" : "rgba(170,60,60,200)")
          .arg(Field_Radius).arg(Field_Pad_V).arg(Field_Pad_H));

    Max_Players_Input->setStyleSheet(QString(
        "QSpinBox {"
        "background-color: rgba(255,255,255,215);"
        "font-weight: bold;"
        "font-family: 'OCR A Extended';"
        "border: 1px solid rgba(80,90,120,120);"
        "border-radius: %1px;"
        "padding: %2px %3px;"
        "color: rgb(15,20,30);"
        "}").arg(Field_Radius).arg(Small_Field_Pad_V).arg(Small_Field_Pad_H));

    Password_Input->setStyleSheet(QString(
        "QLineEdit {"
        "font-family: 'OCR A Extended';"
        "background-color: rgba(255,255,255,215);"
        "border: 1px solid rgba(80,90,120,120);"
        "border-radius: %1px;"
        "padding: %2px %3px;"
        "color: rgb(15,20,30);"
        "}"
        "QLineEdit:disabled {"
        "background-color: rgba(220,220,220,180);"
        "color: rgba(70,70,70,180);"
        "}").arg(Field_Radius).arg(Field_Pad_V).arg(Field_Pad_H));

    const QString Button_Style = QString(
        "QPushButton {"
        "background-color: rgb(160,165,175);"
        "color: rgb(15,20,30);"
        "border: none;"
        "border-radius: %1px;"
        "padding: %2px %3px;"
        "font-weight: 600;"
        "font-family: 'OCR A Extended';"
        "}"
        "QPushButton:hover {"
        "background-color: rgb(180,185,195);"
        "}"
        "QPushButton:pressed {"
        "background-color: rgb(140,145,155);"
        "}"
        "QPushButton:disabled {"
        "background-color: rgba(135,140,150,160);"
        "color: rgba(15,20,30,120);"
        "}").arg(Button_Radius).arg(Button_Pad_V).arg(Button_Pad_H);

    Create_Lobby_Button->setStyleSheet(Button_Style);
    Back_Button->setStyleSheet(Button_Style);
}

void Menu_Create_Lobby_Screen::Update_Create_Button_State()
{
    Create_Lobby_Button->setEnabled(!Host_Name_Input->text().trimmed().isEmpty());
    Apply_Responsive_Styles();
}

void Menu_Create_Lobby_Screen::Set_Button_Cursor_To_Closed_Hand()
{
    QPushButton *Button = qobject_cast<QPushButton*>(sender());

    if(Button != nullptr)
        Button->setCursor(Qt::ClosedHandCursor);
}

void Menu_Create_Lobby_Screen::Set_Button_Cursor_To_Pointing_Hand()
{
    QPushButton *Button = qobject_cast<QPushButton*>(sender());

    if(Button != nullptr)
        Button->setCursor(Qt::PointingHandCursor);
}

void Menu_Create_Lobby_Screen::Handle_Host_Name_Changed(const QString &text)
{
    Q_UNUSED(text);
    Update_Create_Button_State();
}

void Menu_Create_Lobby_Screen::Handle_Public_Lobby_Toggled(bool checked)
{
    const bool Is_Private = !checked;

    Password_Input->setEnabled(Is_Private);
    Show_Password_Checkbox->setEnabled(Is_Private);

    if(!Is_Private)
    {
        Password_Input->clear();
        Password_Input->setEchoMode(QLineEdit::Password);
        Show_Password_Checkbox->setChecked(false);
    }
}

void Menu_Create_Lobby_Screen::Handle_Show_Password_Toggled(bool checked)
{
    if(checked)
        Password_Input->setEchoMode(QLineEdit::Normal);
    else
        Password_Input->setEchoMode(QLineEdit::Password);
}
