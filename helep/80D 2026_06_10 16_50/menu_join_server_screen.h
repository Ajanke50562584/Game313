#ifndef MENU_JOIN_SERVER_SCREEN_H
#define MENU_JOIN_SERVER_SCREEN_H

#include <QWidget> // screen base
#include <QPushButton> // join refresh back buttons
#include <QLineEdit> // player name input
#include <QListWidget> // discovered lobby list
#include <QRegularExpressionValidator> // limits player name typing
#include <QLabel> // title and labels
#include <QVBoxLayout> // vertical layout
#include <QResizeEvent> // responsive sizing

class Menu_Join_Server_Screen : public QWidget // screen where client finds lobbies
{
    Q_OBJECT // Signals and slots

public:
    explicit Menu_Join_Server_Screen(QWidget *parent = nullptr); // builds join screen

    // expose controls the menu manager needs for join flow
    QPushButton *Get_Join_Button() const; // join selected lobby
    QPushButton *Get_Refresh_Button() const; // search for lobbies
    QPushButton *Get_Back_Button() const; // back to multiplayer menu
    QListWidget *Get_Lobby_List() const; // list used by menu manager
    QLineEdit *Get_Player_Name_Input() const; // local player name

protected:
    // resize fonts to match the current window width
    void resizeEvent(QResizeEvent *event) override;

private:
    void Apply_Responsive_Styles(); // resize fonts and style

    // layout and join widgets
    QVBoxLayout *Main_Layout; // whole screen layout
    QWidget *Card_Widget; // central panel
    QVBoxLayout *Card_Layout; // panel layout
    QLabel *Title_Label; // screen title
    QLabel *Sub_Title_Label; // small subtitle
    QLabel *Player_Name_Label; // input label
    QLineEdit *Player_Name_Input; // name typed by client
    QRegularExpressionValidator *Player_Name_Validator; // protects name input
    QListWidget *Lobby_List; // discovered lobbies
    QPushButton *Join_Button; // join selected
    QPushButton *Refresh_Button; // discover again
    QPushButton *Back_Button; // back button

private slots:
    void Set_Button_Cursor_To_Closed_Hand(); // button pressed cursor
    void Set_Button_Cursor_To_Pointing_Hand(); // button released cursor
};

#endif // MENU_JOIN_SERVER_SCREEN_H
