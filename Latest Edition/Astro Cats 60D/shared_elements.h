
#ifndef SHARED_ELEMENTS_H
#define SHARED_ELEMENTS_H

#include <QObject>
#include <QString>
#include <QList>
#include <QHostAddress>

struct Lobby_Info
{
    // Stores info needed to join a lobby
    QString Lobby_Name;
    QString Host_Name;
    QHostAddress Host_Address;

    // Defualt setup
    quint16 Host_Port = 30033;
    int Current_Players = 0;
    int Max_Players = 4;
    bool Is_Public_Lobby = true;
};


struct Player_Info
{
    //Stores the info for a single player defualts
    quint32 Player_ID = 0;
    QString Player_Name;
    bool Is_Host = false;
    bool Is_Ready = false;
};



class Shared_Elements : public QObject
{
public:
    explicit Shared_Elements(QObject *parent = nullptr);

    // Resets all shared lobby and player data back to default values
    void Reset_Lobby_State();


    // Lobby settings setters
    void Set_Lobby_Name(const QString &lobbyName);
    void Set_Max_Players(int maxPlayers);
    void Set_Lobby_Privacy(bool isPublicLobby);


    // Network status information setters
    void Set_Connection_Status(const QString &connectionStatus);
    void Set_Player_Role(const QString &playerRole);
    void Set_Remote_Address(const QString &remoteAddress);
    void Set_Last_Network_Event(const QString &lastNetworkEvent);


    // Current player state setters
    void Set_Is_Hosting(bool isHosting);
    void Set_Is_In_Lobby(bool isInLobby);
    void Set_Local_Player_Name(const QString &playerName);
    void Set_Local_Player_ID(quint32 playerID);
    void Set_Local_Player_Ready(bool isReady);


    // Selected and current lobby information setters
    void Set_Selected_Lobby(const Lobby_Info &lobbyInfo);
    void Set_Current_Lobby_Info(const Lobby_Info &lobbyInfo);


    // Discovered lobby list management
    void Clear_Discovered_Lobbies();
    void Add_Discovered_Lobby(const Lobby_Info &lobbyInfo);


    // Player list management
    void Clear_Players();
    void Add_Player(const Player_Info &playerInfo);
    void Set_Player_List(const QList<Player_Info> &players);

    // Lobby settings
    QString Get_Lobby_Name() const;
    int Get_Max_Players() const;
    bool Get_Is_Public_Lobby() const;

    // Network status information
    QString Get_Connection_Status() const;
    QString Get_Player_Role() const;
    QString Get_Remote_Address() const;
    QString Get_Last_Network_Event() const;

    // Current player state
    bool Get_Is_Hosting() const;
    bool Get_Is_In_Lobby() const;
    QString Get_Local_Player_Name() const;
    quint32 Get_Local_Player_ID() const;
    bool Get_Local_Player_Ready() const;

    // Selected and current lobby information
    Lobby_Info Get_Selected_Lobby() const;
    Lobby_Info Get_Current_Lobby_Info() const;

    // Discovered lobby and player lists
    QList<Lobby_Info> Get_Discovered_Lobbies() const;
    QList<Player_Info> Get_Player_List() const;
private:
    // Lobby settings
    QString Lobby_Name;
    int Max_Players = 4;
    bool Is_Public_Lobby = true;

    // Network status information
    QString Connection_Status = "Not connected";
    QString Player_Role;
    QString Remote_Address = "Not connected";
    QString Last_Network_Event;

    // Current player state
    bool Is_Hosting = false;
    bool Is_In_Lobby = false;
    QString Local_Player_Name;
    quint32 Local_Player_ID = 0;
    bool Local_Player_Ready = false;

    // Selected and current lobby information
    Lobby_Info Selected_Lobby;
    Lobby_Info Current_Lobby_Info;

    // Discovered lobby and player lists
    QList<Lobby_Info> Discovered_Lobbies;
    QList<Player_Info> Players;
};

#endif // SHARED_ELEMENTS_H
