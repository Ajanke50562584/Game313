#ifndef STORY_MODE_MAIN_SCREEN_H
#define STORY_MODE_MAIN_SCREEN_H

#include <QWidget>
#include <QList>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHideEvent>
#include <QResizeEvent>
#include <QShowEvent>
#include <QTimer>
#include <QPixmap>

class Story_Mode_Main_Screen: public QWidget
{
    Q_OBJECT

public:
    explicit Story_Mode_Main_Screen(QWidget *parent = nullptr);
    // multiplayer button for menu navigation
    QPushButton *Get_Tutorial_Button() const;
    QPushButton *Get_Select_Level_Button() const;


protected:
    // resize fonts and character placement with the window
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;

private:
    int Current_Frame;
    int Completed_Animation_Loops;
    bool Animation_Status_Reported;

    QLabel *Story_Mode_Title;
    QVBoxLayout *Main_Layout;

    QList<QPushButton*> Buttons;
    QPushButton *Tutorial_Button;
    QPushButton *Select_Level_Button;

    QLabel *Character_Label;
    QList<QPixmap> Frames;
    QTimer *Story_Mode_Character_Timer;
    QPixmap *Sprite_Sheet;

    void Apply_Responsive_Sizing();
    void Apply_Responsive_Styles();
    void Report_Animation_Error_Once();

private slots:
    void Update_Frame();
    void Set_Button_Cursor_To_Closed_Hand();
    void Set_Button_Cursor_To_Pointing_Hand();



};


#endif // STORY_MODE_MAIN_SCREEN_H
