#include "shared_elements.h"

Shared_Elements::Shared_Elements(QObject *parent)
    : QObject(parent)
{
    // Start with clean default values for a new app session
    Reset_Lobby_State();
}

void Shared_Elements::Reset_Lobby_State()
{
    // Reset lobby settings
    Lobby_Name = "The Last Constellation Lobby";
    Max_Players = 4;
    Is_Public_Lobby = true;

    // Reset network status information
    Connection_Status = "Idle";
    Player_Role = "Offline";
    Remote_Address = "Not connected";
    Last_Network_Event = "No network activity yet";

    // Reset current player state
    Is_Hosting = false;
    Is_In_Lobby = false;
    Local_Player_Name = "Player";
    Local_Player_ID = 0;
    Local_Player_Ready = false;

    // Clear selected lobby and rebuild the current lobby information
    Selected_Lobby = Lobby_Info();
    Current_Lobby_Info = Lobby_Info();

    Selected_Lobby.Lobby_Name = "";
    Current_Lobby_Info.Lobby_Name = Lobby_Name;
    Current_Lobby_Info.Max_Players = Max_Players;
    Current_Lobby_Info.Is_Public_Lobby = Is_Public_Lobby;

    // Clear old lobby and player lists
    Discovered_Lobbies.clear();
    Players.clear();
}


// Lobby settings

void Shared_Elements::Set_Lobby_Name(const QString &lobbyName)
{
    // Use the default lobby name if the user leaves the input empty
    Lobby_Name = lobbyName.trimmed().isEmpty()? QString("The Last Constellation Lobby"): lobbyName.trimmed();

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


// Network status information

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


// Current player state

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
    // Keep a default name if the user leaves the player name blank
    Local_Player_Name = playerName.trimmed().isEmpty() ? QString("Player"): playerName.trimmed();
}

void Shared_Elements::Set_Local_Player_ID(quint32 playerID)
{
    Local_Player_ID = playerID;
}

void Shared_Elements::Set_Local_Player_Ready(bool isReady) // Update the Shared state after the button is pressed
{
    // Store the local ready state directly
    Local_Player_Ready = isReady;

    // Also update this player's entry in the shared player list|
    // Go through player list if match found update it.
    for(int i = 0; i < Players.size(); ++i)
    {
        // Check if player matches the local player by ID
        const bool Matches_ID = (Local_Player_ID != 0 && Players[i].Player_ID == Local_Player_ID);
         // Check if player matches the local player by Name
        const bool Matches_Name = Players[i].Player_Name == Local_Player_Name;

        if(Matches_ID || Matches_Name) // If it matches uupdate their ready state
        {
            Players[i].Is_Ready = isReady;
            return;
        }
    }
}


// Selected and current lobby information

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
}


// Discovered lobby list management

void Shared_Elements::Clear_Discovered_Lobbies()
{
    Discovered_Lobbies.clear();
}

void Shared_Elements::Add_Discovered_Lobby(const Lobby_Info &lobbyInfo)
{
    for(int i = 0; i < Discovered_Lobbies.size(); ++i)
    {
        // Refresh an existing lobby instead of adding a duplicate // check for lobbies alr in list
        if(Discovered_Lobbies[i].Host_Address == lobbyInfo.Host_Address &&
                Discovered_Lobbies[i].Host_Port == lobbyInfo.Host_Port)
        {
            Discovered_Lobbies[i] = lobbyInfo; // If a match is found update its lobby info instead of making a duplicate
            return;
        }
    }

    // Add the lobby if it was not already in the list
    Discovered_Lobbies.append(lobbyInfo);
}


// Player list management

void Shared_Elements::Clear_Players()
{
    Players.clear();
}

void Shared_Elements::Add_Player(const Player_Info &playerInfo)
{
    for(int i = 0; i < Players.size(); ++i)
    {
        // Refresh an existing player instead of adding a duplicate
        if(Players[i].Player_ID == playerInfo.Player_ID)
        {
            Players[i] = playerInfo; // Update player info
            return;
        }
    }

    // Add the player if they were not already in the list
    Players.append(playerInfo);
}

void Shared_Elements::Set_Player_List(const QList<Player_Info> &players)
{
    Players = players;
}


// Lobby setting getters

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


// Network status getters

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


// Current player state getters

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
    return Local_Player_Ready;
}


// Selected and current lobby getters

Lobby_Info Shared_Elements::Get_Selected_Lobby() const
{
    return Selected_Lobby;
}

Lobby_Info Shared_Elements::Get_Current_Lobby_Info() const
{
    return Current_Lobby_Info;
}


// List getters

QList<Lobby_Info> Shared_Elements::Get_Discovered_Lobbies() const
{
    return Discovered_Lobbies;
}

QList<Player_Info> Shared_Elements::Get_Player_List() const
{
    return Players;
}

// These functions are const because they only read the shared state.
// They return stored values without changing any member variables.
