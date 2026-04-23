#ifndef GAME_NETWORKING_ELEMENTS_H
#define GAME_NETWORKING_ELEMENTS_H

#include <QObject>
#include <QHostAddress>
#include <QJsonObject>
#include <QJsonDocument>

class QUdpSocket;
class Shared_Elements;
class Game_Multiplayer_Lobby_Elements;

class Game_Networking_Elements : public QObject
{
public:
    explicit Game_Networking_Elements(QObject *parent = nullptr);

    void Attach_Shared_Elements(Shared_Elements *sharedElements);
    void Attach_Multiplayer_Lobby_Elements(Game_Multiplayer_Lobby_Elements *lobbyElements);

    // ==== SETUP / CORE ====
    bool Start_Listening();
    void Request_To_Join(const QHostAddress &address, quint16 port);
    void Send_Datagram(const QString &Datagram, QHostAddress address, quint16 port);


    // ==== LOBBY DISCOVERY ====
    void Send_Discover_Lobbies();
    void Send_Lobby_Announce(const QHostAddress &address, quint16 port);
    void Process_JSON_Packet(const QJsonObject &obj, const QHostAddress &sender, quint16 senderPort);

private:
    enum Datagram_Type
    {
        UNKNOWN,
        REQUEST_TO_JOIN,
        JOIN_ACCEPTED
    };

    void Read_Pending_Datagrams();
    Datagram_Type Get_Datagram_Type(const QString &Datagram);
    void Refresh_Lobby_View();

    QUdpSocket *Game_UDP_Socket;
    Shared_Elements *Shared_Group;
    Game_Multiplayer_Lobby_Elements *Game_Multiplayer_Lobby_Group;
};

#endif // GAME_NETWORKING_ELEMENTS_H
