#include "shared_elements.h"

Shared_Elements::Shared_Elements(QObject *parent)
    : QObject(parent)
{
    // start with clean defaults for a new app session
    Reset_Lobby_State();
}

void Shared_Elements::Reset_Lobby_State()
{
    // default lobby values before anyone hosts or joins
    Lobby_Name = "The Last Constellation Lobby";
    Max_Players = 4;
    Is_Public_Lobby = true;
    Map_Difficulty = "Medium";
    Lobby_Password = "";
    Easy_Vote_Count = 0;
    Medium_Vote_Count = 0;
    Hard_Vote_Count = 0;
    Connection_Status = "Idle";
    Player_Role = "Offline";
    Remote_Address = "Not connected";
    Last_Network_Event = "No network activity yet";
    Is_Hosting = false;
    Is_In_Lobby = false;

    Local_Player_Name = "Player";
    Local_Player_ID = 0;

    // clear any last-selected / current lobby info
    Selected_Lobby = Lobby_Info();
    Current_Lobby_Info = Lobby_Info();

    Selected_Lobby.Lobby_Name = "";
    Current_Lobby_Info.Lobby_Name = Lobby_Name;
    Current_Lobby_Info.Max_Players = Max_Players;
    Current_Lobby_Info.Is_Public_Lobby = Is_Public_Lobby;
    Current_Lobby_Info.Map_Difficulty = Map_Difficulty;

    Discovered_Lobbies.clear();
    Players.clear();
}

void Shared_Elements::Set_Lobby_Name(const QString &lobbyName)
{
    // fall back to the default name if input was blank
    Lobby_Name = lobbyName.trimmed().isEmpty()
            ? QString("The Last Constellation Lobby")
            : lobbyName.trimmed();

    Current_Lobby_Info.Lobby_Name = Lobby_Name;
}

void Shared_Elements::Set_Max_Players(int maxPlayers)
{
    Max_Players = maxPlayers;
    Current_Lobby_Info.Max_Players = maxPlayers;
}

void Shared_Elements::Set_Lobby_Privacy(bool isPublicLobby)
{
    Is_Public_Lobby = isPublicLobby;
    Current_Lobby_Info.Is_Public_Lobby = isPublicLobby;
}

void Shared_Elements::Set_Map_Difficulty(const QString &mapDifficulty)
{
    QString Clean_Difficulty = mapDifficulty.trimmed();

    if(Clean_Difficulty != "Easy" && Clean_Difficulty != "Medium" && Clean_Difficulty != "Hard")
        Clean_Difficulty = "Medium";

    Map_Difficulty = Clean_Difficulty;
    Current_Lobby_Info.Map_Difficulty = Clean_Difficulty;
}

void Shared_Elements::Set_Lobby_Password(const QString &password)
{
    Lobby_Password = password;
}

void Shared_Elements::Add_Map_Difficulty_Vote(const QString &difficulty)
{
    if(difficulty == "Easy")
        Easy_Vote_Count++;
    else if(difficulty == "Medium")
        Medium_Vote_Count++;
    else if(difficulty == "Hard")
        Hard_Vote_Count++;

    QString Winner = "Easy";
    int Best_Count = Easy_Vote_Count;

    if(Medium_Vote_Count >= Best_Count)
    {
        Winner = "Medium";
        Best_Count = Medium_Vote_Count;
    }

    if(Hard_Vote_Count >= Best_Count)
        Winner = "Hard";

    Set_Map_Difficulty(Winner);
}

void Shared_Elements::Set_Connection_Status(const QString &connectionStatus)
{
    Connection_Status = connectionStatus;
}

void Shared_Elements::Set_Player_Role(const QString &playerRole)
{
    Player_Role = playerRole;
}

void Shared_Elements::Set_Remote_Address(const QString &remoteAddress)
{
    Remote_Address = remoteAddress;
}

void Shared_Elements::Set_Last_Network_Event(const QString &lastNetworkEvent)
{
    Last_Network_Event = lastNetworkEvent;
}

void Shared_Elements::Set_Is_Hosting(bool isHosting)
{
    Is_Hosting = isHosting;
}

void Shared_Elements::Set_Is_In_Lobby(bool isInLobby)
{
    Is_In_Lobby = isInLobby;
}

void Shared_Elements::Set_Local_Player_Name(const QString &playerName)
{
    // keep a fallback name if the user leaves it empty
    Local_Player_Name = playerName.trimmed().isEmpty()
            ? QString("Player")
            : playerName.trimmed();
}

void Shared_Elements::Set_Local_Player_ID(quint32 playerID)
{
    Local_Player_ID = playerID;
}

void Shared_Elements::Set_Local_Player_Ready(bool isReady)
{
    for(int i = 0; i < Players.size(); ++i)
    {
        const bool Matches_ID = (Local_Player_ID != 0 && Players[i].Player_ID == Local_Player_ID);
        const bool Matches_Name = Players[i].Player_Name == Local_Player_Name;

        if(Matches_ID || Matches_Name)
        {
            Players[i].Is_Ready = isReady;
            return;
        }
    }
}

void Shared_Elements::Set_Selected_Lobby(const Lobby_Info &lobbyInfo)
{
    Selected_Lobby = lobbyInfo;
}

void Shared_Elements::Set_Current_Lobby_Info(const Lobby_Info &lobbyInfo)
{
    Current_Lobby_Info = lobbyInfo;
    Lobby_Name = lobbyInfo.Lobby_Name;
    Max_Players = lobbyInfo.Max_Players;
    Is_Public_Lobby = lobbyInfo.Is_Public_Lobby;
    Map_Difficulty = lobbyInfo.Map_Difficulty.trimmed().isEmpty() ? QString("Medium") : lobbyInfo.Map_Difficulty;
}

void Shared_Elements::Clear_Discovered_Lobbies()
{
    Discovered_Lobbies.clear();
}

void Shared_Elements::Add_Discovered_Lobby(const Lobby_Info &lobbyInfo)
{
    for(int i = 0; i < Discovered_Lobbies.size(); ++i)
    {
        // refresh an existing lobby entry instead of duplicating it
        if(Discovered_Lobbies[i].Host_Address == lobbyInfo.Host_Address &&
                Discovered_Lobbies[i].Host_Port == lobbyInfo.Host_Port)
        {
            Discovered_Lobbies[i] = lobbyInfo;
            return;
        }
    }

    // append a new lobby entry
    Discovered_Lobbies.append(lobbyInfo);
}

void Shared_Elements::Clear_Players()
{
    Players.clear();
}

void Shared_Elements::Add_Player(const Player_Info &playerInfo)
{
    Players.append(playerInfo);
}

void Shared_Elements::Remove_Last_Non_Host_Player()
{
    for(int i = Players.size() - 1; i >= 0; i--)
    {
        if(!Players[i].Is_Host)
        {
            Players.removeAt(i);
            return;
        }
    }
}

void Shared_Elements::Set_Player_List(const QList<Player_Info> &players)
{
    Players = players;
}

QString Shared_Elements::Get_Lobby_Name() const
{
    return Lobby_Name;
}

int Shared_Elements::Get_Max_Players() const
{
    return Max_Players;
}

bool Shared_Elements::Get_Is_Public_Lobby() const
{
    return Is_Public_Lobby;
}

QString Shared_Elements::Get_Map_Difficulty() const
{
    return Map_Difficulty;
}

QString Shared_Elements::Get_Connection_Status() const
{
    return Connection_Status;
}

QString Shared_Elements::Get_Player_Role() const
{
    return Player_Role;
}

QString Shared_Elements::Get_Remote_Address() const
{
    return Remote_Address;
}

QString Shared_Elements::Get_Last_Network_Event() const
{
    return Last_Network_Event;
}

bool Shared_Elements::Get_Is_Hosting() const
{
    return Is_Hosting;
}

bool Shared_Elements::Get_Is_In_Lobby() const
{
    return Is_In_Lobby;
}

QString Shared_Elements::Get_Local_Player_Name() const
{
    return Local_Player_Name;
}

quint32 Shared_Elements::Get_Local_Player_ID() const
{
    return Local_Player_ID;
}

bool Shared_Elements::Get_Local_Player_Ready() const
{
    for(int i = 0; i < Players.size(); ++i)
    {
        const bool Matches_ID = (Local_Player_ID != 0 && Players[i].Player_ID == Local_Player_ID);
        const bool Matches_Name = Players[i].Player_Name == Local_Player_Name;

        if(Matches_ID || Matches_Name)
            return Players[i].Is_Ready;
    }

    return false;
}

Lobby_Info Shared_Elements::Get_Selected_Lobby() const
{
    return Selected_Lobby;
}

Lobby_Info Shared_Elements::Get_Current_Lobby_Info() const
{
    return Current_Lobby_Info;
}

QList<Lobby_Info> Shared_Elements::Get_Discovered_Lobbies() const
{
    return Discovered_Lobbies;
}

QList<Player_Info> Shared_Elements::Get_Player_List() const
{
    return Players;
}

QString Shared_Elements::Get_Lobby_Password() const
{
    return Lobby_Password;
}

int Shared_Elements::Get_Easy_Votes() const
{
    return Easy_Vote_Count;
}

int Shared_Elements::Get_Medium_Votes() const
{
    return Medium_Vote_Count;
}

int Shared_Elements::Get_Hard_Votes() const
{
    return Hard_Vote_Count;
}
