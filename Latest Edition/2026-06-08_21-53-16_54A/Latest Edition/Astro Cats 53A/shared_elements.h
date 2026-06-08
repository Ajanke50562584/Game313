
#ifndef SHARED_ELEMENTS_H
#define SHARED_ELEMENTS_H

#include <QObject>
#include <QString>
#include <QList>
#include <QHostAddress>

struct Lobby_Info
{
    // lobby info shared across screens
    QString Lobby_Name;
    QString Host_Name;
    QHostAddress Host_Address;
    quint16 Host_Port;
    int Current_Players;
    int Max_Players;
    bool Is_Public_Lobby;


    //setup defualt info if host has not changed the lobby settings
    Lobby_Info()
        : Host_Port(30033) // cannot be changed
        , Current_Players(0)
        , Max_Players(4)
        , Is_Public_Lobby(true)
    {
    }
};

struct Player_Info
{
    // one player entry in shared state
    quint32 Player_ID;
    QString Player_Name;
    bool Is_Host;
    bool Is_Ready;

    Player_Info()
        : Player_ID(0)
        , Is_Host(false)
        , Is_Ready(false)
    {
    }
};

class Shared_Elements : public QObject
{
public:
    explicit Shared_Elements(QObject *parent = nullptr);

    // reset lobby and player data to defaults
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

    void Set_Local_Player_Name(const QString &playerName);
    void Set_Local_Player_ID(quint32 playerID);
    void Set_Local_Player_Ready(bool isReady);
    void Set_Selected_Lobby(const Lobby_Info &lobbyInfo);
    void Set_Current_Lobby_Info(const Lobby_Info &lobbyInfo);

    void Clear_Discovered_Lobbies();
    void Add_Discovered_Lobby(const Lobby_Info &lobbyInfo);
    void Clear_Players();
    void Add_Player(const Player_Info &playerInfo);
    void Set_Player_List(const QList<Player_Info> &players);

    QString Get_Lobby_Name() const;
    int Get_Max_Players() const;
    bool Get_Is_Public_Lobby() const;
    QString Get_Connection_Status() const;
    QString Get_Player_Role() const;
    QString Get_Remote_Address() const;
    QString Get_Last_Network_Event() const;
    bool Get_Is_Hosting() const;
    bool Get_Is_In_Lobby() const;

    QString Get_Local_Player_Name() const;
    quint32 Get_Local_Player_ID() const;
    bool Get_Local_Player_Ready() const;
    Lobby_Info Get_Selected_Lobby() const;
    Lobby_Info Get_Current_Lobby_Info() const;
    QList<Lobby_Info> Get_Discovered_Lobbies() const;
    QList<Player_Info> Get_Player_List() const;

private:
    // current lobby state
    QString Lobby_Name;
    int Max_Players;
    bool Is_Public_Lobby;
    QString Connection_Status;
    QString Player_Role;
    QString Remote_Address;
    QString Last_Network_Event;
    bool Is_Hosting;
    bool Is_In_Lobby;

    // local player info
    QString Local_Player_Name;
    quint32 Local_Player_ID;

    // selected and active lobby copies
    Lobby_Info Selected_Lobby;
    Lobby_Info Current_Lobby_Info;

    // discovered lobbies and current players
    QList<Lobby_Info> Discovered_Lobbies;
    QList<Player_Info> Players;
};

#endif // SHARED_ELEMENTS_H
