#include "menu_maker.h"
#include <QApplication>
#include <QFont>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>

Menu_Maker::Menu_Maker(QWidget* parent)
    :QWidget(parent)
{
    Main_Layout = new QVBoxLayout(this);

    //Setting the Game title at the top

    Game_Title = new QLabel("Astro Cats: <br>  The Last Constellation",this);
    Game_Title->setAlignment(Qt::AlignCenter);
    Game_Title->setWordWrap(true);
    Game_Title->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

    //Creating the Push Buttons
    QPushButton *Story_Mode_button = new QPushButton("Story Mode",this);
    QPushButton *Multi_Player_button = new QPushButton("Multi Player",this);
    QPushButton *View_Highscore_button = new QPushButton("View Highscores",this);
    QPushButton *Exit_Game_Button = new QPushButton("Exit Game",this);

    //Adding them buttons to the list
    Buttons.append(Story_Mode_button);
    Buttons.append(Multi_Player_button);
    Buttons.append(View_Highscore_button);
    Buttons.append(Exit_Game_Button);

    // Making Everything scal to full screen
    for(int i=0;i< Buttons.size();i++)
    {
        Buttons[i]->setMinimumHeight(55);
        Buttons[i]->setMaximumWidth(500);
        Buttons[i]->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);

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
                    "}"
                    );
    }

    //Stylization
    setStyleSheet("background: transparent;");

    Game_Title->setStyleSheet(
                "color: rgb(235,235,245);"
                "background-color: rgba(80,80,100,80);"
                "border: 2px solid rgba(200,200,230,120);"
                "border-radius: 18px;"
                "padding: 10px 20px;"
                );

    // Layout settings

    QHBoxLayout *Title_Row = new QHBoxLayout();
    Title_Row->addStretch(1);
    Title_Row->addWidget(Game_Title, 3);
    Title_Row->addStretch(1);

    Main_Layout->addLayout(Title_Row);
    Main_Layout->addSpacing(20);
    Main_Layout->addStretch(1);

    for(int i=0;i< Buttons.size();i++)
        Main_Layout->addWidget(Buttons[i],0,Qt::AlignHCenter);

    Main_Layout->addStretch(2);

    // Making the Exit game Actually Exit the Game

    connect(Exit_Game_Button, &QPushButton::clicked , qApp, &QApplication::quit);

}


void Menu_Maker::resizeEvent(QResizeEvent *event)
{
    int w = width();

    QFont Title_Font;
    Title_Font.setPointSize(w/40);
    Game_Title->setFont(Title_Font);

    QFont Push_Button_Font;
    Push_Button_Font.setPointSize(w/65);
    for(int i=0;i< Buttons.size();i++)
        Buttons[i]->setFont(Push_Button_Font);

    QWidget::resizeEvent(event);
}
