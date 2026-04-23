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
    , Shared_Group(nullptr)
    , Game_Multiplayer_Lobby_Group(nullptr)
{
    // make the udp socket
    Game_UDP_Socket = new QUdpSocket(this);

    // whenever something arrives, read it
    connect(Game_UDP_Socket, &QUdpSocket::readyRead, this, [this]() {
        Read_Pending_Datagrams();
    });
}

void Game_Networking_Elements::Attach_Shared_Elements(Shared_Elements *sharedElements)
{
    // store shared data reference
    Shared_Group = sharedElements;
}

void Game_Networking_Elements::Attach_Multiplayer_Lobby_Elements(Game_Multiplayer_Lobby_Elements *lobbyElements)
{
    // store lobby ui reference
    Game_Multiplayer_Lobby_Group = lobbyElements;
}

bool Game_Networking_Elements::Start_Listening()
{
    if(Game_UDP_Socket == nullptr)
        return false;

    // already bound? dont do it again
    if(Game_UDP_Socket->state() == QAbstractSocket::BoundState)
        return true;

    // bind to port 30033 so we can receive stuff
    const bool ok = Game_UDP_Socket->bind(QHostAddress::AnyIPv4,
                                          30033,
                                          QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);

    if(Shared_Group != nullptr)
    {
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
        // this is a client now
        Shared_Group->Set_Is_Hosting(false);
        Shared_Group->Set_Is_In_Lobby(true);
        Shared_Group->Set_Player_Role("Client");

        Shared_Group->Set_Connection_Status("sending join request");
        Shared_Group->Set_Remote_Address(address.toString() + ":" + QString::number(port));
        Shared_Group->Set_Last_Network_Event("join request sent");
    }

    // send simple string join request
    Send_Datagram("REQUEST_TO_JOIN", address, port);

    Refresh_Lobby_View();
}

void Game_Networking_Elements::Send_Datagram(const QString &Datagram, QHostAddress address, quint16 port)
{
    if(Game_UDP_Socket == nullptr)
        return;

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

        // fallback to string packets
        QString data = QString::fromUtf8(datagram);

        switch(Get_Datagram_Type(data))
        {
        case REQUEST_TO_JOIN:
        {
            qDebug() << "join request received";

            // only host should handle this
            if(Shared_Group != nullptr && Shared_Group->Get_Is_Hosting())
            {
                // update some status text
                Shared_Group->Set_Connection_Status("player joined");
                Shared_Group->Set_Remote_Address(sender.toString() + ":" + QString::number(senderPort));
                Shared_Group->Set_Last_Network_Event("join request accepted");

                // add player if not already in list
                QList<Player_Info> players = Shared_Group->Get_Player_List();

                bool exists = false;
                for(int i = 0; i < players.size(); i++)
                {
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

                // tell client they are in
                Send_Datagram("JOIN_ACCEPTED", sender, senderPort);

                // send full player list as json
                QJsonObject root;
                root["type"] = "PLAYER_LIST";

                QJsonArray arr;

                QList<Player_Info> updated = Shared_Group->Get_Player_List();

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

        Refresh_Lobby_View();
    }
}

void Game_Networking_Elements::Refresh_Lobby_View()
{
    // just tell ui to update
    if(Game_Multiplayer_Lobby_Group != nullptr)
        Game_Multiplayer_Lobby_Group->Refresh_From_Shared_Elements();
}

void Game_Networking_Elements::Send_Discover_Lobbies()
{
    if(Game_UDP_Socket == nullptr)
        return;

    // ask everyone for lobbies
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

    // send lobby info back to the requester
    QJsonObject obj;
    obj["type"] = "LOBBY_ANNOUNCE";
    obj["lobbyName"] = Shared_Group->Get_Lobby_Name();
    obj["hostName"] = Shared_Group->Get_Local_Player_Name();
    obj["currentPlayers"] = Shared_Group->Get_Player_List().size();
    obj["maxPlayers"] = Shared_Group->Get_Max_Players();

    QJsonDocument doc(obj);
    QByteArray data = doc.toJson(QJsonDocument::Compact);

    Game_UDP_Socket->writeDatagram(data, address, port);

    qDebug() << "sent lobby announce";
}

void Game_Networking_Elements::Process_JSON_Packet(const QJsonObject &obj,
                                                   const QHostAddress &sender,
                                                   quint16 senderPort)
{
    if(Shared_Group == nullptr)
        return;

    QString type = obj["type"].toString();

    if(type == "DISCOVER_LOBBIES")
    {
        // only host replies
        if(Shared_Group->Get_Is_Hosting())
            Send_Lobby_Announce(sender, senderPort);
    }
    else if(type == "LOBBY_ANNOUNCE")
    {
        // store discovered lobby
        Lobby_Info lobby;
        lobby.Lobby_Name = obj["lobbyName"].toString();
        lobby.Host_Name = obj["hostName"].toString();
        lobby.Host_Address = sender;
        lobby.Host_Port = senderPort;
        lobby.Current_Players = obj["currentPlayers"].toInt();
        lobby.Max_Players = obj["maxPlayers"].toInt();

        Shared_Group->Add_Discovered_Lobby(lobby);
    }
    else if(type == "PLAYER_LIST")
    {
        // rebuild player list from host
        QList<Player_Info> players;

        QJsonArray arr = obj["players"].toArray();

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

    Refresh_Lobby_View();
}

Game_Networking_Elements::Datagram_Type Game_Networking_Elements::Get_Datagram_Type(const QString &Datagram)
{
    if(Datagram == "REQUEST_TO_JOIN")
        return REQUEST_TO_JOIN;

    if(Datagram == "JOIN_ACCEPTED")
        return JOIN_ACCEPTED;

    return UNKNOWN;
}
