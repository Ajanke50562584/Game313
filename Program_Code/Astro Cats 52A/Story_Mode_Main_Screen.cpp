#include "story_mode_main_screen.h"

#include <QApplication>
#include <QFont>
#include <QHideEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QResizeEvent>
#include <QShowEvent>
#include <QSizePolicy>
#include <QVBoxLayout>
#include <QFont>
#include <QPixmap>
#include <QDebug>
#include <QTimer>


Story_Mode_Main_Screen::Story_Mode_Main_Screen(QWidget *parent)
    :QWidget(parent)
    , Current_Frame(0)
    , Completed_Animation_Loops(0)
    , Animation_Status_Reported(false)
{
    // main layout for title and buttons
    Main_Layout = new QVBoxLayout(this);

    //Set the background to an image
    setAttribute(Qt::WA_StyledBackground, true);
    QPixmap test(":/Images/Background/Space_BG_6.png");
    qDebug() << test.isNull();

    // cut the sprite sheet into frames
    Sprite_Sheet = new QPixmap(":/Images/Characters/Standing_Wagging_Gray.png"); // change to one of the gray ones
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

    // character in lower corner
    Character_Label = new QLabel(this);
    Character_Label->setAttribute(Qt::WA_TranslucentBackground);
    if (!Frames.isEmpty())
    {
        Character_Label->setPixmap(Frames[0]);
        qDebug() << "first frame size:" << Frames[0].size();
    }

    // timer for the character animation
    Story_Mode_Character_Timer = new QTimer(this);
    connect(Story_Mode_Character_Timer, &QTimer::timeout,this, &Story_Mode_Main_Screen::Update_Frame);
    Story_Mode_Character_Timer->start(150);

    //background
    setObjectName("MainMenu");
    setStyleSheet(
        "#MainMenu {"
        "border-image: url(:/Images/Background/Space_BG_6.png) 0 0 0 0 stretch stretch;"
        "}"
    );

    //Story mode title
    Story_Mode_Title = new QLabel("Welcome to Story Mode", this);
    Story_Mode_Title->setAlignment(Qt::AlignCenter);
    Story_Mode_Title->setWordWrap(true);
    Story_Mode_Title->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    //navigation buttons
    Tutorial_Button = new QPushButton("Tutorial", this);
    Select_Level_Button = new QPushButton("Select Memory Fragment", this);

    // keep the main buttons together for styling and font updates
    Buttons.append(Tutorial_Button);
    Buttons.append(Select_Level_Button);
    for(int i = 0; i < Buttons.size(); i++)
        Buttons[i]->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    // center the title row
    QHBoxLayout *Title_Row = new QHBoxLayout();
    Title_Row->addStretch(1);
    Title_Row->addWidget(Story_Mode_Title, 3);
    Title_Row->addStretch(1);

    Main_Layout->addLayout(Title_Row);
    Main_Layout->addSpacing(20);
    Main_Layout->addStretch(1);


    // add buttons and pointer feedback
    for(int i = 0; i < Buttons.size(); i++)
    {
        Main_Layout->addWidget(Buttons[i], 0, Qt::AlignHCenter);
        Buttons[i]->setCursor(Qt::PointingHandCursor);
        connect(Buttons[i], SIGNAL(pressed()), this, SLOT(Set_Button_Cursor_To_Closed_Hand()));
        connect(Buttons[i], SIGNAL(released()), this, SLOT(Set_Button_Cursor_To_Pointing_Hand()));
    }

    Main_Layout->addStretch(2);


    Apply_Responsive_Sizing();

}




QPushButton *Story_Mode_Main_Screen::Get_Tutorial_Button() const
{
    return Tutorial_Button;
}

QPushButton *Story_Mode_Main_Screen::Get_Select_Level_Button() const
{
    return  Select_Level_Button;
}

void Story_Mode_Main_Screen::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    Apply_Responsive_Sizing();
}

void Story_Mode_Main_Screen::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);

    if(Story_Mode_Character_Timer != nullptr && !Story_Mode_Character_Timer->isActive())
    {
        Story_Mode_Character_Timer->start(150);
        qDebug() << "Animation started: Main menu";
    }
}

void Story_Mode_Main_Screen::hideEvent(QHideEvent *event)
{
    QWidget::hideEvent(event);

    if(Story_Mode_Character_Timer != nullptr && Story_Mode_Character_Timer->isActive())
    {
        Story_Mode_Character_Timer->stop();
        qDebug() << "Animation stopped: Main menu";
    }
}

void Story_Mode_Main_Screen::Update_Frame()
{
    if (Frames.isEmpty())
    {
        Report_Animation_Error_Once();
        return;
    }

    // scale the current frame before the next one
    QPixmap scaled = Frames[Current_Frame].scaled(
            Character_Label->size(),
            Qt::KeepAspectRatio,
            Qt::FastTransformation
        );
    Character_Label->setPixmap(Frames[Current_Frame]);
    Character_Label->setPixmap(scaled);

    Current_Frame = (Current_Frame + 1) % Frames.size();

    if(Current_Frame == 0)
    {
        Completed_Animation_Loops++;

        if(Completed_Animation_Loops >= 3 && !Animation_Status_Reported)
        {
            Animation_Status_Reported = true;
            qDebug() << "Animation successful: Main menu";
        }
    }
}

void Story_Mode_Main_Screen::Apply_Responsive_Sizing()
{
    const int w = width();
    const int h = height();

    Main_Layout->setContentsMargins(qMax(12, (24 * w) / 1280),
                                    qMax(12, (24 * h) / 720),
                                    qMax(12, (24 * w) / 1280),
                                    qMax(12, (24 * h) / 720));
    Main_Layout->setSpacing(qMax(8, (16 * h) / 720));

    QFont Title_Font("OCR A Extended");
    Title_Font.setPointSize(qMax(18, w / 40));
    Story_Mode_Title->setFont(Title_Font);

    QFont Push_Button_Font("OCR A Extended");
    Push_Button_Font.setPointSize(qMax(10, w / 65));

    const int Button_Height = qMax(40, (55 * h) / 720);
    const int Button_Max_Width = qMax(220, (500 * w) / 1280);

    for(int i = 0; i < Buttons.size(); i++)
    {
        Buttons[i]->setFont(Push_Button_Font);
        Buttons[i]->setMinimumHeight(Button_Height);
        Buttons[i]->setMaximumWidth(Button_Max_Width);
    }

    if(Character_Label != nullptr)
    {
        Character_Label->resize(qMax(96, (384 * w) / 1280),
                                qMax(96, (384 * h) / 720));
        Character_Label->move(qMax(12, (40 * w) / 1280),
                              height() - Character_Label->height() - qMax(12, (40 * h) / 720));
        Character_Label->raise();
    }

    Apply_Responsive_Styles();
}

void Story_Mode_Main_Screen::Apply_Responsive_Styles()
{
    const int w = qMax(width(), 640);
    const int h = qMax(height(), 360);
    const int Button_Radius = qMax(14, (26 * h) / 720);
    const int Button_Padding_V = qMax(6, (10 * h) / 720);
    const int Button_Padding_H = qMax(14, (24 * w) / 1280);
    const int Title_Radius = qMax(12, (18 * h) / 720);
    const int Title_Padding_V = qMax(6, (10 * h) / 720);
    const int Title_Padding_H = qMax(12, (20 * w) / 1280);

    for(int i = 0; i < Buttons.size(); i++)
    {
        Buttons[i]->setStyleSheet(QString(
            "QPushButton {"
            "background-color: rgba(210,210,220,135);"
            "color: rgb(20,20,30);"
            "border: 2px solid rgba(245,245,255,190);"
            "border-radius: %1px;"
            "padding: %2px %3px;"
            "}"
            "QPushButton:hover {"
            "background-color: rgba(235,235,245,175);"
            "border: 2px solid rgba(255,255,255,230);"
            "}"
            "QPushButton:pressed {"
            "background-color: rgba(180,180,195,190);"
            "}")
            .arg(Button_Radius)
            .arg(Button_Padding_V)
            .arg(Button_Padding_H));
    }

    Story_Mode_Title->setStyleSheet(QString(
        "color: rgb(235,235,245);"
        "background-color: rgba(108, 56, 70, 237);"
        "border: 2px solid rgba(200,200,230,120);"
        "border-radius: %1px;"
        "padding: %2px %3px;")
        .arg(Title_Radius)
        .arg(Title_Padding_V)
        .arg(Title_Padding_H));
}

void Story_Mode_Main_Screen::Report_Animation_Error_Once()
{
    if(Animation_Status_Reported)
        return;

    Animation_Status_Reported = true;
    qDebug() << "Animation error: Main menu";
}

void Story_Mode_Main_Screen::Set_Button_Cursor_To_Closed_Hand()
{
    QPushButton *Button = qobject_cast<QPushButton*>(sender());

    if(Button != nullptr)
        Button->setCursor(Qt::ClosedHandCursor);
}

void Story_Mode_Main_Screen::Set_Button_Cursor_To_Pointing_Hand()
{
    QPushButton *Button = qobject_cast<QPushButton*>(sender());

    if(Button != nullptr)
        Button->setCursor(Qt::PointingHandCursor);
}
