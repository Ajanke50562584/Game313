#include "leaderboard_screen.h"
#include "leaderboard_data.h"

#include <QFont>
#include <QHBoxLayout>
#include <QShowEvent>

Leaderboard_Screen::Leaderboard_Screen(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground, true);
    setObjectName("LeaderboardScreen");
    setStyleSheet("#LeaderboardScreen { border-image: url(:/Images/Background/Space_BG_5.png) 0 0 0 0 stretch stretch; }");

    Main_Layout = new QVBoxLayout(this);

    Title_Label = new QLabel("Leaderboard", this);
    Title_Label->setAlignment(Qt::AlignCenter);

    Leaderboard_Text = new QTextEdit(this);
    Leaderboard_Text->setReadOnly(true);

    Back_Button = new QPushButton("Back", this);
    Back_Button->setCursor(Qt::PointingHandCursor);

    QHBoxLayout *Title_Row = new QHBoxLayout();
    Title_Row->addStretch(1);
    Title_Row->addWidget(Title_Label, 3);
    Title_Row->addStretch(1);

    QHBoxLayout *Back_Row = new QHBoxLayout();
    Back_Row->addStretch(1);
    Back_Row->addWidget(Back_Button);
    Back_Row->addStretch(1);

    Main_Layout->addLayout(Title_Row);
    Main_Layout->addWidget(Leaderboard_Text);
    Main_Layout->addLayout(Back_Row);

    Refresh_Leaderboard();
    Apply_Responsive_Styles();
}

QPushButton *Leaderboard_Screen::Get_Back_Button() const
{
    return Back_Button;
}

void Leaderboard_Screen::Refresh_Leaderboard()
{
    QList<Leaderboard_Entry> Entries = Leaderboard_Data::Get_Entries();

    QString Text;
    if(Entries.isEmpty())
    {
        Text = "No course results yet.";
    }
    else
    {
        for(int i = 0; i < Entries.size(); i++)
        {
            Text += QString("%1. %2    %3    %4\n")
                    .arg(i + 1)
                    .arg(Entries[i].Player_Name)
                    .arg(Leaderboard_Data::Format_Time(Entries[i].Time_Ms))
                    .arg(Entries[i].Result);
        }
    }

    Leaderboard_Text->setPlainText(Text);
}

void Leaderboard_Screen::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    Apply_Responsive_Styles();
}

void Leaderboard_Screen::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    Refresh_Leaderboard();
}

void Leaderboard_Screen::Apply_Responsive_Styles()
{
    const int w = qMax(width(), 640);
    const int h = qMax(height(), 360);

    Main_Layout->setContentsMargins(qMax(12, (24 * w) / 1280),
                                    qMax(12, (24 * h) / 720),
                                    qMax(12, (24 * w) / 1280),
                                    qMax(12, (24 * h) / 720));
    Main_Layout->setSpacing(qMax(8, (16 * h) / 720));

    QFont Title_Font("OCR A Extended");
    Title_Font.setPointSize(qMax(18, w / 40));
    Title_Label->setFont(Title_Font);

    QFont Text_Font("OCR A Extended");
    Text_Font.setPointSize(qMax(10, w / 80));
    Leaderboard_Text->setFont(Text_Font);
    Back_Button->setFont(Text_Font);

    Title_Label->setStyleSheet("color: rgb(235,235,245);"
                               "background-color: rgba(0,74,113,237);"
                               "border: 2px solid rgba(200,200,230,120);"
                               "border-radius: 14px;"
                               "padding: 10px;");

    Leaderboard_Text->setStyleSheet("background-color: rgba(15,20,35,210);"
                                    "color: rgb(235,235,245);"
                                    "border: 2px solid rgba(245,245,255,160);"
                                    "border-radius: 10px;"
                                    "padding: 12px;");

    Back_Button->setMinimumHeight(qMax(40, (55 * h) / 720));
    Back_Button->setMaximumWidth(qMax(220, (500 * w) / 1280));
    Back_Button->setStyleSheet("QPushButton {"
                               "background-color: rgba(210,210,220,150);"
                               "color: rgb(20,20,30);"
                               "border: 2px solid rgba(245,245,255,190);"
                               "border-radius: 14px;"
                               "padding: 8px 20px;"
                               "}"
                               "QPushButton:hover {"
                               "background-color: rgba(235,235,245,185);"
                               "}");
}
