#ifndef SHARED_ELEMENTS_H
#define SHARED_ELEMENTS_H

#include <QObject>
#include <QString>

class Shared_Elements : public QObject
{
public:
    explicit Shared_Elements(QObject *parent = nullptr);

    void Reset_Lobby_State();
    void Set_Lobby_Name(const QString &lobbyName);
    void Set_Max_Players(int maxPlayers);
    void Set_Lobby_Privacy(bool isPublicLobby);
    void Set_Connection_Status(const QString &connectionStatus);
    void Set_Player_Role(const QString &playerRole);
    void Set_Remote_Address(const QString &remoteAddress);
    void Set_Last_Network_Event(const QString &lastNetworkEvent);
    void Set_Is_Hosting(bool isHosting);
    void Set_Is_In_Lobby(bool isInLobby);

    QString Get_Lobby_Name() const;
    int Get_Max_Players() const;
    bool Get_Is_Public_Lobby() const;
    QString Get_Connection_Status() const;
    QString Get_Player_Role() const;
    QString Get_Remote_Address() const;
    QString Get_Last_Network_Event() const;
    bool Get_Is_Hosting() const;
    bool Get_Is_In_Lobby() const;

private:
    QString Lobby_Name;
    int Max_Players;
    bool Is_Public_Lobby;
    QString Connection_Status;
    QString Player_Role;
    QString Remote_Address;
    QString Last_Network_Event;
    bool Is_Hosting;
    bool Is_In_Lobby;
};

#endif // SHARED_ELEMENTS_H
