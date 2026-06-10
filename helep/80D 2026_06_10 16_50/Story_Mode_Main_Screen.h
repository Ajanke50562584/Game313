#ifndef STORY_MODE_MAIN_SCREEN_H
#define STORY_MODE_MAIN_SCREEN_H

#include <QWidget> // screen base
#include <QList> // button and frame lists
#include <QLabel> // title and character label
#include <QPushButton> // story buttons
#include <QVBoxLayout> // vertical layout
#include <QGridLayout>
#include <QHideEvent> // stop animation hidden
#include <QResizeEvent> // responsive sizing
#include <QShowEvent> // restart animation shown
#include <QTimer> // animation timer
#include <QPixmap> // sprite sheet frames

class Story_Mode_Main_Screen: public QWidget // story mode level select screen
{
    Q_OBJECT // Signals and slots

public:
    explicit Story_Mode_Main_Screen(QWidget *parent = nullptr); // builds story menu
    QPushButton *Get_Tutorial_Button() const; // tutorial level button
    QPushButton *Get_Select_Level_Button() const; // normal level button
    QList<QPushButton*> Get_Level_Buttons() const;
    QPushButton *Get_Back_Button() const; // back to main menu


protected:
    // resize fonts and character placement with the window
    void resizeEvent(QResizeEvent *event) override; // resize fonts and character placement
    void showEvent(QShowEvent *event) override; // start animation
    void hideEvent(QHideEvent *event) override; // stop animation

private:
    int Current_Frame; // current animation frame
    int Completed_Animation_Loops; // debug loop count
    bool Animation_Status_Reported; // avoids repeated debug messages

    QLabel *Story_Mode_Title; // story screen title
    QVBoxLayout *Main_Layout; // main screen layout

    QList<QPushButton*> Buttons; // all buttons for shared styling
    QWidget *Level_Grid_Widget;
    QGridLayout *Level_Grid_Layout;
    QPushButton *Tutorial_Button; // opens tutorial
    QPushButton *Select_Level_Button; // opens course level
    QPushButton *Back_Button; // back to main
    QList<QPushButton*> Level_Buttons;

    QLabel *Character_Label; // animated cat label
    QList<QPixmap> Frames; // frames from sprite sheet
    QTimer *Story_Mode_Character_Timer; // animation timer
    QPixmap *Sprite_Sheet; // source image

    void Apply_Responsive_Sizing(); // resize and move widgets
    void Apply_Responsive_Styles(); // styles and fonts
    void Report_Animation_Error_Once(); // warn once if animation broken

private slots:
    void Update_Frame(); // advance animation
    void Set_Button_Cursor_To_Closed_Hand(); // button pressed cursor
    void Set_Button_Cursor_To_Pointing_Hand(); // button released cursor



};


#endif // STORY_MODE_MAIN_SCREEN_H
