#ifndef GAME_NETWORKING_ELEMENTS_H
#define GAME_NETWORKING_ELEMENTS_H

#include <QObject>
#include <QHostAddress>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUdpSocket>

#include "shared_elements.h"
#include "game_multiplayer_lobby_elements.h"

class Game_Networking_Elements : public QObject
{
    Q_OBJECT

public:
    explicit Game_Networking_Elements(QObject *parent = nullptr);

    // shared state ref for networking updates
    void Attach_Shared_Elements(Shared_Elements *sharedElements);

    // lobby screen ref for ui refreshes
    void Attach_Multiplayer_Lobby_Elements(Game_Multiplayer_Lobby_Elements *lobbyElements);

    // bind the udp socket if needed
    bool Start_Listening();

    // send a join request to a host
    void Request_To_Join(const QHostAddress &address, quint16 port);

    // send a string datagram
    void Send_Datagram(const QString &Datagram, QHostAddress address, quint16 port);


    // broadcast a lobby discovery packet
    void Send_Discover_Lobbies();

    // send this host's lobby info back
    void Send_Lobby_Announce(const QHostAddress &address, quint16 port);

    // handle json packet types
    void Process_JSON_Packet(const QJsonObject &obj, const QHostAddress &sender, quint16 senderPort);

    //deals with messages for the chat
    void Send_Chat_Message(const QString &Message);

private:
    // string packet types for join flow
    enum Datagram_Type
    {
        UNKNOWN,
        REQUEST_TO_JOIN,
        JOIN_ACCEPTED
    };

    // read all waiting datagrams
    void Read_Pending_Datagrams();

    // map string packet names to enum values
    Datagram_Type Get_Datagram_Type(const QString &Datagram);

    // refresh lobby ui from shared state
    void Refresh_Lobby_View();

private slots:
    void Handle_Pending_Datagrams();

private:
    // socket + linked systems
    QUdpSocket *Game_UDP_Socket;
    Shared_Elements *Shared_Group;
    Game_Multiplayer_Lobby_Elements *Game_Multiplayer_Lobby_Group;
};

#endif // GAME_NETWORKING_ELEMENTS_H
