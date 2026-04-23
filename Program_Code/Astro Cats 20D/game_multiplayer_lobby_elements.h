#ifndef GAME_MULTIPLAYER_LOBBY_ELEMENTS_H
#define GAME_MULTIPLAYER_LOBBY_ELEMENTS_H

#include <QObject>

class QWidget;
class QLabel;
class Shared_Elements;

class Game_Multiplayer_Lobby_Elements : public QObject
{
public:
    explicit Game_Multiplayer_Lobby_Elements(QWidget *parent = nullptr);

    void Attach_Shared_Elements(Shared_Elements *sharedElements);
    void Refresh_From_Shared_Elements();
    QWidget *Get_Lobby_Widget() const;

private:
    QWidget *Lobby_Widget;
    QLabel *Lobby_Name_Label;
    QLabel *Connection_Status_Label;
    QLabel *Player_Role_Label;
    QLabel *Remote_Address_Label;
    QLabel *Lobby_Type_Label;
    QLabel *Max_Players_Label;
    QLabel *Last_Event_Label;
    Shared_Elements *Shared_Group;
};

#endif // GAME_MULTIPLAYER_LOBBY_ELEMENTS_H
