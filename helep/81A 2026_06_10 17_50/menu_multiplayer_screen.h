#ifndef MENU_MULTIPLAYER_SCREEN_H
#define MENU_MULTIPLAYER_SCREEN_H

#include <QWidget> // screen base
#include <QLabel> // character label
#include <QPushButton> // host/client/back buttons
#include <QHideEvent> // stop animation hidden
#include <QResizeEvent> // responsive sizing
#include <QShowEvent> // restart animation shown
#include <QTimer> // animation timer
#include <QPixmap> // sprite sheet frames

class Menu_Multiplayer_Screen : public QWidget // choose host or client
{
    Q_OBJECT // Signals and slots

public:
    explicit Menu_Multiplayer_Screen(QWidget *parent = nullptr); // builds multiplayer menu

    // menu buttons for navigation
    QPushButton *Get_Host_Button() const; // opens create lobby
    QPushButton *Get_Client_Button() const; // opens join lobby
    QPushButton *Get_Back_Button() const; // back to main menu

private:
    // button refs and animation data
    int Current_Frame; // current animation frame
    int Completed_Animation_Loops; // debug loop count
    bool Animation_Status_Reported; // avoids repeated messages
    QPushButton *Host_Button; // host option
    QPushButton *Client_Button; // join option
    QPushButton *Back_Button; // back option
    QLabel *Character_Label; // animated cat
    QList<QPixmap> Frames; // sprite frames
    QTimer *Multiplayer_Character_Timer; // frame timer
    QPixmap *Sprite_Sheet; // source sprite sheet

    void Apply_Responsive_Sizing(); // move/resize widgets
    void Apply_Responsive_Styles(); // fonts and styles
    void Report_Animation_Error_Once(); // only warn once

protected:
    // keep the character anchored during resize
    void resizeEvent(QResizeEvent *event) override; // keep the character anchored during resize
    void showEvent(QShowEvent *event) override; // restart animation
    void hideEvent(QHideEvent *event) override; // stop animation

private slots:
    // advance the idle animation
    void Update_Frame(); // advance the idle animation
    void Set_Button_Cursor_To_Closed_Hand(); // button pressed cursor
    void Set_Button_Cursor_To_Pointing_Hand(); // button released cursor
};

#endif // MENU_MULTIPLAYER_SCREEN_H
