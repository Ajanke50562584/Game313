#ifndef MENU_MAIN_SCREEN_H
#define MENU_MAIN_SCREEN_H

#include <QWidget> // screen base
#include <QList> // button and frame lists
#include <QLabel> // title and character
#include <QPushButton> // menu buttons
#include <QVBoxLayout> // vertical menu layout
#include <QHideEvent> // stop animation when hidden
#include <QResizeEvent> // responsive sizing
#include <QShowEvent> // restart animation when shown
#include <QTimer> // animation timer
#include <QPixmap> // sprite frames

class Menu_Main_Screen : public QWidget // first screen player sees
{
    Q_OBJECT // Signals and slots

public:
    explicit Menu_Main_Screen(QWidget *parent = nullptr); // builds main menu

    // multiplayer button for menu navigation
    QPushButton *Get_Multiplayer_Button() const; // opens multiplayer menu
    QPushButton *Get_Story_Mode_Button() const; // opens story mode menu
    QPushButton *Get_Highscores_Button() const; // opens highscores


protected:
    // resize fonts and character placement with the window
    void resizeEvent(QResizeEvent *event) override; // resize fonts and character placement
    void showEvent(QShowEvent *event) override; // starts animation again
    void hideEvent(QHideEvent *event) override; // stops animation when not visible

private:
    // title screen widgets and animation data
    int Current_Frame; // current sprite frame
    int Completed_Animation_Loops; // used for debug success message
    bool Animation_Status_Reported; // stops repeated debug spam

    QLabel *Game_Title; // game title label
    QVBoxLayout *Main_Layout; // screen layout

    QList<QPushButton*> Buttons; // all buttons for shared styling
    QPushButton *Multiplayer_Button; // multiplayer menu
    QPushButton *Story_Mode_Button; // story mode
    QPushButton *View_Highscore_Button; // highscore screen

    QLabel *Character_Label; // animated cat label
    QList<QPixmap> Frames; // frames cut from sprite sheet
    QTimer *Menu_Character_Timer; // controls animation speed
    QPixmap *Sprite_Sheet; // full animation sheet

    void Apply_Responsive_Sizing(); // positions widgets based on window size
    void Apply_Responsive_Styles(); // stylesheet and fonts
    void Report_Animation_Error_Once(); // logs if sprite sheet failed

private slots:
    // advance the menu animation
    void Update_Frame(); // advance the menu animation
    void Set_Button_Cursor_To_Closed_Hand(); // button pressed cursor
    void Set_Button_Cursor_To_Pointing_Hand(); // button released cursor
};

#endif // MENU_MAIN_SCREEN_H
