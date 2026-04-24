#ifndef GAME_MULTIPLAYER_LOBBY_ELEMENTS_H
#define GAME_MULTIPLAYER_LOBBY_ELEMENTS_H

#include <QObject>
#include <QList>
#include <QStringList>

// forward declarations
class QWidget;
class QLabel;
class QPushButton;
class QTextEdit;
class QLineEdit;
class QVBoxLayout;
class QScrollArea;
class QTimer;
class Shared_Elements;

class Game_Multiplayer_Lobby_Elements : public QObject
{
public:
    explicit Game_Multiplayer_Lobby_Elements(QWidget *parent = nullptr);

    // attach shared data so this screen can read live lobby info
    void Attach_Shared_Elements(Shared_Elements *sharedElements);

    // updates labels / visibility from shared state
    void Refresh_From_Shared_Elements();

    // root widget for stacked widget system
    QWidget *Get_Lobby_Widget() const;

private:
    // creates one single-line player row
    QLabel *Create_Player_Row(const QString &playerLineText);

    // animates the dots at the end of each player status
    void Update_Player_Status_Ellipses();

    // ===== ROOT =====
    QWidget *Lobby_Widget;

    // shared state
    Shared_Elements *Shared_Group;

    // ===== MAIN AREAS =====
    QWidget *Top_Left_Panel;      // player list
    QWidget *Top_Middle_Panel;    // lobby name + connection status
    QWidget *Top_Right_Panel;     // host controls //chat
    QWidget *Bottom_Left_Panel;   // action buttons //nothing?
    QWidget *Bottom_Right_Panel;  // chat //host controls
    QWidget *Bottom_Middle_Panel; // action buttons
    QWidget *Middle_Middle_Panel; // character demo
    QWidget *Middle_Left_Panel; // nothing
    QWidget *Middle_Right_Panel; // nothing


    // ===== TOP MIDDLE =====
    QLabel *Lobby_Name_Label;
    QLabel *Connection_Status_Label;

    // ===== PLAYER LIST =====
    QScrollArea *Player_List_Scroll_Area;
    QWidget *Player_List_Container;
    QVBoxLayout *Player_List_Layout;

    // keep these so we can animate the ellipses
    QList<QLabel*> Player_Row_Labels;
    QStringList Player_Row_Base_Texts;

    // ===== HOST CONTROLS =====
    QLabel *Host_Options_Title_Label;
    QPushButton *Start_Game_Button;
    QPushButton *Edit_Lobby_Settings_Button;
    QPushButton *Kick_Player_Button;

    // ===== BOTTOM BUTTONS =====
    QPushButton *Leave_Lobby_Button;
    QPushButton *Customize_Character_Button;
    QPushButton *Vote_Map_Button;

    // ===== CHAT =====
    QTextEdit *Chat_Display;
    QLineEdit *Chat_Input;
    QPushButton *Send_Chat_Button;

    // ===== DOT ANIMATION =====
    QTimer *Ellipsis_Timer;
    int Ellipsis_Frame;
};

#endif // GAME_MULTIPLAYER_LOBBY_ELEMENTS_H
