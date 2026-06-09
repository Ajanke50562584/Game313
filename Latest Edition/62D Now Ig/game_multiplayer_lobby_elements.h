#ifndef GAME_MULTIPLAYER_LOBBY_ELEMENTS_H
#define GAME_MULTIPLAYER_LOBBY_ELEMENTS_H

#include <QObject> // signals and slots
#include <QList> // player row lables
#include <QStringList> // text in row labels
#include <QWidget> // to ensure can be moved and placed like a widget
#include <QLabel>
#include <QPushButton>
#include <QTextEdit> // Chat display
#include <QLineEdit> // Chat input
#include <QVBoxLayout> // vertical layout
#include <QScrollArea> // player list
#include <QTimer> // elipses dots
#include <QEvent>

#include "shared_elements.h" // Loby can read and write player data
#include "game_networking_elements.h" // networking class used by lobby

class Game_Multiplayer_Lobby_Elements : public QObject // QObject since it controls widgets i.e. is not controlled by smt else
{
    Q_OBJECT // Signals and slots

public:
    explicit Game_Multiplayer_Lobby_Elements(QWidget *parent = nullptr); // defines the constructer for the lobby


    void Attach_Shared_Elements(Shared_Elements *sharedElements); // Gvie lobby access to gamestate


    void Attach_Networking_Elements(Game_Networking_Elements *networkingElements); // Give lobby access to networking
    void Add_Chat_Message(const QString &message); // Add meassage in chat display from network
    void Refresh_From_Shared_Elements(); // Updates lobby from latest game state

    QWidget *Get_Lobby_Widget() const; // used by other classes to put in stacked widget

signals:
    void Leave_Lobby_Requested(); // send leave to lobby when player presses button
    void Start_Game_Requested();
    void Send_Game_Start();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override; // Checks for window resizing

private:
    // creates one player row
    QLabel *Create_Player_Row(const QString &playerLineText);

    // animates the dots at the end of each player status
    void Update_Player_Status_Ellipses();
    void Apply_Responsive_Sizing(); // updates based on window size
    void Apply_Responsive_Styles();
    bool Get_Local_Player_Ready_State() const; // says if local player is ready

    Game_Networking_Elements *Networking_Group; // holds networking group

private slots:
    void Animate_Player_Status_Ellipses(); // callled by elipses timer
    void Send_To_Chat(); // called when send
    void Handle_Leave_Lobby(); // reset loby state emit leave lobby
    void Handle_Customize_Character();
    void Handle_Vote_Map();
    void Handle_Ready_Toggled();
    void Handle_Start_Game(); // Host button to start game
    void Handle_Edit_Lobby_Settings(); // host controll
    void Handle_Kick_Player(); // host controll

private:
    QWidget *Lobby_Widget; // Actual visible lobby screen


    Shared_Elements *Shared_Group;  // Store state object


    QWidget *Top_Left_Panel;      // player list
    QWidget *Top_Middle_Panel;    // lobby name + ready button
    QWidget *Top_Right_Panel;     // chat
    QWidget *Bottom_Left_Panel;   // host controls
    QWidget *Bottom_Middle_Panel; // action buttons
    QWidget *Middle_Middle_Panel; // character demo



    // top middle
    QLabel *Lobby_Name_Label;
    QPushButton *Ready_Button;

    // player list
    QScrollArea *Player_List_Scroll_Area;
    QWidget *Player_List_Container;
    QVBoxLayout *Player_List_Layout; // vertical to store rows


    QList<QLabel*> Player_Row_Labels;
    QStringList Player_Row_Base_Texts;// without elipses

    // host controls
    QLabel *Host_Options_Title_Label;
    QPushButton *Start_Game_Button;
    QPushButton *Edit_Lobby_Settings_Button;
    QPushButton *Kick_Player_Button;

    // lobby buttons
    QPushButton *Leave_Lobby_Button;
    QPushButton *Customize_Character_Button;
    QPushButton *Vote_Map_Button;

    // chat box
    QTextEdit *Chat_Display;
    QLineEdit *Chat_Input;
    QPushButton *Send_Chat_Button;

    // timer for player status dots
    QTimer *Ellipsis_Timer;
    int Ellipsis_Frame;
};

#endif // GAME_MULTIPLAYER_LOBBY_ELEMENTS_H
