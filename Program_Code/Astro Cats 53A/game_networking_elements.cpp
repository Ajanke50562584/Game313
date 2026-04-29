#include "game_networking_elements.h"
#include "game_multiplayer_lobby_elements.h"
#include "shared_elements.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUdpSocket>

Game_Networking_Elements::Game_Networking_Elements(QObject *parent)
    : QObject(parent)
    , Game_UDP_Socket(nullptr)
    , Shared_Group(nullptr)
    , Game_Multiplayer_Lobby_Group(nullptr)
{
    // create the udp socket
    Game_UDP_Socket = new QUdpSocket(this);

    // read packets as they arrive
    connect(Game_UDP_Socket, SIGNAL(readyRead()), this, SLOT(Handle_Pending_Datagrams()));
}

void Game_Networking_Elements::Attach_Shared_Elements(Shared_Elements *sharedElements)
{
    // store shared state ref
    Shared_Group = sharedElements;
}

void Game_Networking_Elements::Attach_Multiplayer_Lobby_Elements(Game_Multiplayer_Lobby_Elements *lobbyElements)
{
    // store lobby ui ref
    Game_Multiplayer_Lobby_Group = lobbyElements;
}

bool Game_Networking_Elements::Start_Listening()
{
    if(Game_UDP_Socket == nullptr)
        return false;

    // if the socket is already bound, leave it alone
    if(Game_UDP_Socket->state() == QAbstractSocket::BoundState)
        return true;

    // bind to port 30033 for incoming packets
    const bool ok = Game_UDP_Socket->bind(QHostAddress::AnyIPv4,
                                          30033,
                                          QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);

    if(ok)
        Connected_Client_Endpoints.clear();

    if(Shared_Group != nullptr)
    {
        // keep shared labels in sync with socket state
        Shared_Group->Set_Connection_Status(ok ? "Listening on port 30033"
                                               : "Failed to bind port 30033");

        Shared_Group->Set_Remote_Address(ok ? "0.0.0.0:30033"
                                            : "Not connected");

        Shared_Group->Set_Last_Network_Event(ok ? "socket bound"
                                               : "bind failed");
    }

    Refresh_Lobby_View();
    return ok;
}

void Game_Networking_Elements::Request_To_Join(const QHostAddress &address, quint16 port)
{
    if(Shared_Group != nullptr)
    {
        // mark this side as a client
        Shared_Group->Set_Is_Hosting(false);
        Shared_Group->Set_Is_In_Lobby(true);
        Shared_Group->Set_Player_Role("Client");

        Shared_Group->Set_Connection_Status("sending join request");
        Shared_Group->Set_Remote_Address(address.toString() + ":" + QString::number(port));
        Shared_Group->Set_Last_Network_Event("join request sent");
    }

    // send the join request packet
    Send_Datagram("REQUEST_TO_JOIN", address, port);

    Refresh_Lobby_View();
}

void Game_Networking_Elements::Send_Datagram(const QString &Datagram, QHostAddress address, quint16 port)
{
    if(Game_UDP_Socket == nullptr)
        return;

    // string packets still handle the join handshake
    QByteArray data = Datagram.toUtf8();
    Game_UDP_Socket->writeDatagram(data, address, port);
}

void Game_Networking_Elements::Read_Pending_Datagrams()
{
    while(Game_UDP_Socket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(Game_UDP_Socket->pendingDatagramSize());

        QHostAddress sender;
        quint16 senderPort = 0;

        Game_UDP_Socket->readDatagram(datagram.data(),
                                     datagram.size(),
                                     &sender,
                                     &senderPort);

        qDebug() << "got datagram:" << datagram;
        qDebug() << "from ip:" << sender.toString();
        qDebug() << "from port:" << senderPort;

        // try json first
        QJsonDocument doc = QJsonDocument::fromJson(datagram);

        if(doc.isObject())
        {
            Process_JSON_Packet(doc.object(), sender, senderPort);
            continue;
        }

        // otherwise fall back to string packets
        QString data = QString::fromUtf8(datagram);

        switch(Get_Datagram_Type(data))
        {
        case REQUEST_TO_JOIN:
        {
            qDebug() << "join request received";

            // only host should handle this
            if(Shared_Group != nullptr && Shared_Group->Get_Is_Hosting())
            {
                // update shared status fields
                Shared_Group->Set_Connection_Status("player joined");
                Shared_Group->Set_Remote_Address(sender.toString() + ":" + QString::number(senderPort));
                Shared_Group->Set_Last_Network_Event("join request accepted");
                Remember_Client_Endpoint(sender, senderPort);

                // add the player if not already in the list
                QList<Player_Info> players = Shared_Group->Get_Player_List();

                bool exists = false;
                for(int i = 0; i < players.size(); i++)
                {
                    // use sender port as a temporary player id
                    if(players[i].Player_ID == senderPort)
                    {
                        exists = true;
                        break;
                    }
                }

                if(!exists)
                {
                    Player_Info p;
                    p.Player_ID = senderPort;
                    p.Player_Name = "Player_" + QString::number(senderPort);
                    p.Is_Host = false;
                    p.Is_Ready = false;

                    Shared_Group->Add_Player(p);
                }

                // tell the client the join was accepted
                Send_Datagram("JOIN_ACCEPTED", sender, senderPort);

                // send the full player list as json
                QJsonObject root;
                root["type"] = "PLAYER_LIST";

                QJsonArray arr;

                QList<Player_Info> updated = Shared_Group->Get_Player_List();

                // send the updated list so the client can mirror it
                for(int i = 0; i < updated.size(); i++)
                {
                    QJsonObject obj;
                    obj["playerID"] = (int)updated[i].Player_ID;
                    obj["playerName"] = updated[i].Player_Name;
                    obj["isHost"] = updated[i].Is_Host;
                    obj["isReady"] = updated[i].Is_Ready;
                    arr.append(obj);
                }

                root["players"] = arr;

                QJsonDocument out(root);
                QByteArray bytes = out.toJson(QJsonDocument::Compact);

                Game_UDP_Socket->writeDatagram(bytes, sender, senderPort);
            }

            Refresh_Lobby_View();
            break;
        }

        case JOIN_ACCEPTED:
            qDebug() << "join accepted";

            if(Shared_Group != nullptr)
            {
                Shared_Group->Set_Connection_Status("connected to host");
                Shared_Group->Set_Remote_Address(sender.toString() + ":" + QString::number(senderPort));
                Shared_Group->Set_Last_Network_Event("host accepted join");
                Shared_Group->Set_Is_In_Lobby(true);
            }
            break;

        default:
            qDebug() << "unknown packet";
            break;
        }

        // any packet can change the lobby screen
        Refresh_Lobby_View();
    }
}

void Game_Networking_Elements::Refresh_Lobby_View()
{
    // refresh the lobby screen
    if(Game_Multiplayer_Lobby_Group != nullptr)
        Game_Multiplayer_Lobby_Group->Refresh_From_Shared_Elements();
}

void Game_Networking_Elements::Send_Discover_Lobbies()
{
    if(Game_UDP_Socket == nullptr)
        return;

    // ask the local network for lobbies
    QJsonObject obj;
    obj["type"] = "DISCOVER_LOBBIES";

    QJsonDocument doc(obj);
    QByteArray data = doc.toJson(QJsonDocument::Compact);

    Game_UDP_Socket->writeDatagram(data, QHostAddress::Broadcast, 30033);

    qDebug() << "sent discover";
}

void Game_Networking_Elements::Send_Lobby_Announce(const QHostAddress &address, quint16 port)
{
    if(Game_UDP_Socket == nullptr || Shared_Group == nullptr)
        return;

    // package host lobby info into a reply
    QJsonObject obj;
    obj["type"] = "LOBBY_ANNOUNCE";
    obj["lobbyName"] = Shared_Group->Get_Lobby_Name();
    obj["hostName"] = Shared_Group->Get_Local_Player_Name();
    obj["currentPlayers"] = Shared_Group->Get_Player_List().size();
    obj["maxPlayers"] = Shared_Group->Get_Max_Players();

    QJsonDocument doc(obj);
    QByteArray data = doc.toJson(QJsonDocument::Compact);

    Game_UDP_Socket->writeDatagram(data, address, port);

    qDebug() << "sent lobby announce to" << address.toString() << port;
}

void Game_Networking_Elements::Process_JSON_Packet(const QJsonObject &obj,
                                                   const QHostAddress &sender,
                                                   quint16 senderPort)
{
    if(Shared_Group == nullptr)
        return;

    QString type = obj["type"].toString();

    qDebug() << "json packet type:" << type;
    qDebug() << "json sender ip:" << sender.toString();
    qDebug() << "json sender port:" << senderPort;

    if(type == "DISCOVER_LOBBIES")
    {
        qDebug() << "DISCOVER_LOBBIES received";
        qDebug() << "am I hosting?" << Shared_Group->Get_Is_Hosting();

        if(Shared_Group->Get_Is_Hosting())
        {
            qDebug() << "sending lobby announce back";
            Send_Lobby_Announce(sender, senderPort);
        }
        else
        {
            qDebug() << "ignored discover because not hosting";
        }
    }
    else if(type == "LOBBY_ANNOUNCE")
    {
        qDebug() << "LOBBY_ANNOUNCE received";

        Lobby_Info lobby;
        lobby.Lobby_Name = obj["lobbyName"].toString();
        lobby.Host_Name = obj["hostName"].toString();
        lobby.Host_Address = sender;
        lobby.Host_Port = senderPort;
        lobby.Current_Players = obj["currentPlayers"].toInt();
        lobby.Max_Players = obj["maxPlayers"].toInt();

        // store this lobby or refresh the old entry
        Shared_Group->Add_Discovered_Lobby(lobby);
    }
    else if(type == "PLAYER_LIST")
    {
        qDebug() << "PLAYER_LIST received";

        QList<Player_Info> players;
        QJsonArray arr = obj["players"].toArray();

        // rebuild the local player list from json
        for(int i = 0; i < arr.size(); i++)
        {
            QJsonObject p = arr[i].toObject();

            Player_Info info;
            info.Player_ID = p["playerID"].toInt();
            info.Player_Name = p["playerName"].toString();
            info.Is_Host = p["isHost"].toBool();
            info.Is_Ready = p["isReady"].toBool();

            players.append(info);
        }

        Shared_Group->Set_Player_List(players);
    }
    else if(type == "CHAT")
    {
        const QString senderName = obj["senderName"].toString("Player");
        const QString message = obj["message"].toString().trimmed();

        if(message.isEmpty())
            return;

        qDebug() << "CHAT received from" << senderName << ":" << message;

        if(Game_Multiplayer_Lobby_Group != nullptr)
            Game_Multiplayer_Lobby_Group->Add_Chat_Message(senderName + ": " + message);

        if(Shared_Group->Get_Is_Hosting())
        {
            for(int i = 0; i < Connected_Client_Endpoints.size(); ++i)
            {
                const QPair<QHostAddress, quint16> &endpoint = Connected_Client_Endpoints[i];

                if(endpoint.first == sender && endpoint.second == senderPort)
                    continue;

                Send_Chat_Packet(senderName, message, endpoint.first, endpoint.second);
            }
        }
    }

    // refresh the lobby screen after json packets too
    Refresh_Lobby_View();
}

void Game_Networking_Elements::Send_Chat_Message(const QString &Message)
{
    qDebug() << "Chat message to send:" << Message;

    if(Game_UDP_Socket == nullptr || Shared_Group == nullptr)
        return;

    const QString senderName = Shared_Group->Get_Local_Player_Name();

    if(Shared_Group->Get_Is_Hosting())
    {
        for(int i = 0; i < Connected_Client_Endpoints.size(); ++i)
        {
            const QPair<QHostAddress, quint16> &endpoint = Connected_Client_Endpoints[i];
            Send_Chat_Packet(senderName, Message, endpoint.first, endpoint.second);
        }
        return;
    }

    const Lobby_Info selectedLobby = Shared_Group->Get_Selected_Lobby();
    if(!selectedLobby.Host_Address.isNull() && selectedLobby.Host_Port != 0)
    {
        Send_Chat_Packet(senderName, Message, selectedLobby.Host_Address, selectedLobby.Host_Port);
        return;
    }

    qDebug() << "No host endpoint available for chat";
}


Game_Networking_Elements::Datagram_Type Game_Networking_Elements::Get_Datagram_Type(const QString &Datagram)
{
    // string packet mapping for join flow
    if(Datagram == "REQUEST_TO_JOIN")
        return REQUEST_TO_JOIN;

    if(Datagram == "JOIN_ACCEPTED")
        return JOIN_ACCEPTED;

    return UNKNOWN;
}

void Game_Networking_Elements::Handle_Pending_Datagrams()
{
    Read_Pending_Datagrams();
}

void Game_Networking_Elements::Send_Chat_Packet(const QString &senderName,
                                                const QString &message,
                                                const QHostAddress &address,
                                                quint16 port)
{
    if(Game_UDP_Socket == nullptr)
        return;

    QJsonObject obj;
    obj["type"] = "CHAT";
    obj["senderName"] = senderName;
    obj["message"] = message;

    const QJsonDocument doc(obj);
    const QByteArray bytes = doc.toJson(QJsonDocument::Compact);
    Game_UDP_Socket->writeDatagram(bytes, address, port);
}

void Game_Networking_Elements::Remember_Client_Endpoint(const QHostAddress &address, quint16 port)
{
    for(int i = 0; i < Connected_Client_Endpoints.size(); ++i)
    {
        if(Connected_Client_Endpoints[i].first == address &&
           Connected_Client_Endpoints[i].second == port)
            return;
    }

    Connected_Client_Endpoints.append(qMakePair(address, port));
}
