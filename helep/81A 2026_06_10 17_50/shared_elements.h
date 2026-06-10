
#ifndef SHARED_ELEMENTS_H
#define SHARED_ELEMENTS_H

#include <QObject> // shared object base
#include <QString> // all text fields
#include <QList> // player and lobby lists
#include <QHostAddress> // host ip address

struct Lobby_Info // info about one discoverable lobby
{
    QString Lobby_Name; // name shown in lobby list
    QString Host_Name; // name of host player
    QHostAddress Host_Address; // ip address of host
    quint16 Host_Port; // udp port used by lobby
    int Current_Players; // players joined now
    int Max_Players; // player limit
    bool Is_Public_Lobby; // can be discovered
    QString Map_Difficulty; // Easy Medium or Hard


    //setup defualt info if host has not changed the lobby settings
    Lobby_Info()
    {
        Host_Port = 30033; // cannot be changed
        Current_Players = 0;
        Max_Players = 4;
        Is_Public_Lobby = true;
        Map_Difficulty = "Medium";
    }
};

struct Player_Info // one player entry in shared state
{
    quint32 Player_ID; // id used for network/list rows
    QString Player_Name; // visible player name
    bool Is_Host; // true for lobby host
    bool Is_Ready; // ready toggle

    Player_Info() // safe defaults for new players
    {
        Player_ID = 0;
        Is_Host = false;
        Is_Ready = false;
    }
};

class Shared_Elements : public QObject // shared game/lobby data store
{
public:
    explicit Shared_Elements(QObject *parent = nullptr); // creates default shared state

    // reset lobby and player data to defaults
    void Reset_Lobby_State();

    void Set_Lobby_Name(const QString &lobbyName); // change lobby name
    void Set_Max_Players(int maxPlayers); // change lobby player limit
    void Set_Lobby_Privacy(bool isPublicLobby); // public/private toggle
    void Set_Map_Difficulty(const QString &mapDifficulty); // selected map difficulty
    void Set_Connection_Status(const QString &connectionStatus); // text for ui status
    void Set_Player_Role(const QString &playerRole); // Host or Client
    void Set_Remote_Address(const QString &remoteAddress); // address text shown to player
    void Set_Last_Network_Event(const QString &lastNetworkEvent); // debugging/status event
    void Set_Is_Hosting(bool isHosting); // true if this machine hosts
    void Set_Is_In_Lobby(bool isInLobby); // true once inside lobby

    void Set_Local_Player_Name(const QString &playerName); // local name
    void Set_Local_Player_ID(quint32 playerID); // local id
    void Set_Local_Player_Ready(bool isReady); // local ready toggle
    void Set_Selected_Lobby(const Lobby_Info &lobbyInfo); // lobby selected in join list
    void Set_Current_Lobby_Info(const Lobby_Info &lobbyInfo); // active lobby copy

    void Clear_Discovered_Lobbies(); // remove old search results
    void Add_Discovered_Lobby(const Lobby_Info &lobbyInfo); // add or update found lobby
    void Clear_Players(); // empty player list
    void Add_Player(const Player_Info &playerInfo); // add one player
    void Remove_Last_Non_Host_Player(); // host kick helper
    void Set_Player_List(const QList<Player_Info> &players); // replace whole player list
    void Set_Lobby_Password(const QString &password); // private lobby password
    void Add_Map_Difficulty_Vote(const QString &difficulty); // add one vote and retally

    QString Get_Lobby_Name() const; // current lobby name
    int Get_Max_Players() const; // player limit
    bool Get_Is_Public_Lobby() const; // public/private state
    QString Get_Map_Difficulty() const; // selected map difficulty
    QString Get_Connection_Status() const; // ui status text
    QString Get_Player_Role() const; // host/client text
    QString Get_Remote_Address() const; // remote address display
    QString Get_Last_Network_Event() const; // latest network status
    bool Get_Is_Hosting() const; // hosting flag
    bool Get_Is_In_Lobby() const; // lobby flag

    QString Get_Local_Player_Name() const; // player name on this pc
    quint32 Get_Local_Player_ID() const; // player id on this pc
    bool Get_Local_Player_Ready() const; // local ready state
    Lobby_Info Get_Selected_Lobby() const; // selected join lobby
    Lobby_Info Get_Current_Lobby_Info() const; // active lobby info
    QList<Lobby_Info> Get_Discovered_Lobbies() const; // found lobbies
    QList<Player_Info> Get_Player_List() const; // players in lobby
    QString Get_Lobby_Password() const; // lobby password
    int Get_Easy_Votes() const;
    int Get_Medium_Votes() const;
    int Get_Hard_Votes() const;

private:
    // current lobby state
    QString Lobby_Name; // name of active lobby
    int Max_Players; // max players allowed
    bool Is_Public_Lobby; // public if true
    QString Map_Difficulty; // difficulty selected in lobby
    QString Lobby_Password; // password for private lobby
    int Easy_Vote_Count;
    int Medium_Vote_Count;
    int Hard_Vote_Count;
    QString Connection_Status; // connection label text
    QString Player_Role; // Host/Client
    QString Remote_Address; // address label text
    QString Last_Network_Event; // debug event text
    bool Is_Hosting; // host mode
    bool Is_In_Lobby; // currently in lobby

    // local player info
    QString Local_Player_Name; // this player's name
    quint32 Local_Player_ID; // this player's id

    // selected and active lobby copies
    Lobby_Info Selected_Lobby; // chosen from join list
    Lobby_Info Current_Lobby_Info; // lobby being hosted/joined

    // discovered lobbies and current players
    QList<Lobby_Info> Discovered_Lobbies; // search results
    QList<Player_Info> Players; // current player list
};

#endif // SHARED_ELEMENTS_H
