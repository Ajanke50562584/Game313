#ifndef MENU_CREATE_LOBBY_SCREEN_H
#define MENU_CREATE_LOBBY_SCREEN_H

#include <QWidget> // screen base
#include <QPushButton> // create/back buttons
#include <QLineEdit> // text inputs
#include <QCheckBox> // public and password checkbox
#include <QSpinBox> // max players selector
#include <QLabel> // form labels
#include <QVBoxLayout> // stacks form widgets
#include <QResizeEvent> // responsive sizing
#include <QRegularExpressionValidator> // To restrict what the players can typre into the text feilds

class Menu_Create_Lobby_Screen : public QWidget // screen for host to setup lobby
{
    Q_OBJECT // Signals and slots

public:
    explicit Menu_Create_Lobby_Screen(QWidget *parent = nullptr); // builds create lobby form

    // Getters Galore once more
    QPushButton *Get_Create_Lobby_Button() const; // create button for menu manager
    QPushButton *Get_Back_Button() const; // back button for menu manager
    QLineEdit *Get_Lobby_Name_Input() const; // lobby name entered
    QLineEdit *Get_Host_Name_Input() const; // host/player name entered
    QSpinBox *Get_Max_Players_Input() const; // max players chosen
    QCheckBox *Get_Public_Lobby_Checkbox() const; // public/private setting

protected:
    // resize fonts to fit the current window size
    void resizeEvent(QResizeEvent *event) override;

private:
    void Apply_Responsive_Styles(); // update fonts and stylesheet
    void Update_Create_Button_State(); // disable create if form is invalid

private slots:
    // Handlers to control gamestate
    void Handle_Host_Name_Changed(const QString &text); // validates host name
    void Handle_Public_Lobby_Toggled(bool checked); // show/hide password bits
    void Handle_Show_Password_Toggled(bool checked); // password visibility
    void Set_Button_Cursor_To_Closed_Hand(); // button pressed cursor
    void Set_Button_Cursor_To_Pointing_Hand(); // button released cursor

private:
    // layout and form widgets
    QVBoxLayout *Main_Layout; // whole screen layout
    QWidget *Card_Widget; // form panel
    QVBoxLayout *Card_Layout; // inside form panel
    QLabel *Title_Label; // screen title
    QLabel *Sub_Title_Label; // small helper text
    QLabel *Lobby_Name_Label; // lobby name label
    QLabel *Host_Name_Label; // host name label
    QLabel *Max_Players_Label; // player count label
    QLabel *Password_Label; // password label
    QLineEdit *Lobby_Name_Input; // lobby name textbox
    QLineEdit *Host_Name_Input; // host name textbox
    QSpinBox *Max_Players_Input; // player count spinner
    QCheckBox *Public_Lobby_Checkbox; // public/private toggle
    QLineEdit *Password_Input; // password text
    QCheckBox *Show_Password_Checkbox; // reveal password toggle
    QPushButton *Create_Lobby_Button; // creates lobby
    QPushButton *Back_Button; // return to multiplayer menu

    QRegularExpressionValidator *Lobby_Name_Validator; // limits lobby name typing
    QRegularExpressionValidator *Host_Name_Validator; // limits host name typing
};

#endif // MENU_CREATE_LOBBY_SCREEN_H
