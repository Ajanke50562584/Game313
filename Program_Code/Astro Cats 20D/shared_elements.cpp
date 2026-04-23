#include "shared_elements.h"

Shared_Elements::Shared_Elements(QObject *parent)
    : QObject(parent)
{
    Reset_Lobby_State();
}

void Shared_Elements::Reset_Lobby_State()
{
    Lobby_Name = "The Last Constellation Lobby";
    Max_Players = 4;
    Is_Public_Lobby = true;
    Connection_Status = "Idle";
    Player_Role = "Offline";
    Remote_Address = "Not connected";
    Last_Network_Event = "No network activity yet";
    Is_Hosting = false;
    Is_In_Lobby = false;
}

void Shared_Elements::Set_Lobby_Name(const QString &lobbyName)
{
    Lobby_Name = lobbyName.trimmed().isEmpty() ? QString("The Last Constellation Lobby") : lobbyName.trimmed();
}

void Shared_Elements::Set_Max_Players(int maxPlayers)
{
    Max_Players = maxPlayers;
}

void Shared_Elements::Set_Lobby_Privacy(bool isPublicLobby)
{
    Is_Public_Lobby = isPublicLobby;
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
