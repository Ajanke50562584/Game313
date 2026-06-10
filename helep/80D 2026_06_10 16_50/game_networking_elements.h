#ifndef GAME_NETWORKING_ELEMENTS_H
#define GAME_NETWORKING_ELEMENTS_H

#include <QObject> // signals and slots
#include <QHostAddress> // ip addresses
#include <QJsonObject> // packet data
#include <QJsonDocument> // json serialization
#include <QUdpSocket> // udp network socket
#include <QTimer> // repeated lobby announce
#include <QList> // endpoint list
#include <QPair> // address and port pair

#include "shared_elements.h" // networking reads/writes shared state

class Game_Networking_Elements : public QObject // handles all udp lobby networking
{
    Q_OBJECT // Signals and slots

public:
    explicit Game_Networking_Elements(QObject *parent = nullptr); // creates socket and timers

    // shared state ref for networking updates
    void Attach_Shared_Elements(Shared_Elements *sharedElements);

    // lobby screen ref for ui refreshes
    void Attach_Multiplayer_Lobby_Elements(QObject *lobbyElements);

    bool Start_Listening(); // bind as host on lobby port
    bool Start_Client_Listening(); // bind as client so replies can arrive

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

    void Send_Chat_Message(const QString &Message); //deals with messages for the chat

    void Send_Game_Start(); // Start multiplayer
    void Send_Map_Vote(const QString &difficulty); // send a difficulty vote

signals:
        void Game_Start_Received(); // tells menus to switch to game screen


private:
    void Send_Chat_Packet(const QString &senderName,
                          const QString &message,
                          const QHostAddress &address,
                          quint16 port); // send one chat packet to one endpoint
    void Remember_Client_Endpoint(const QHostAddress &address, quint16 port); // store joined client

    // sends this host's lobby info to all local network broadcasts
    void Send_Lobby_Announce_Broadcasts();

    enum Datagram_Type // old string packet types
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
    void Handle_Pending_Datagrams(); // called when socket has packets


private:
    // socket + linked systems
    QUdpSocket *Game_UDP_Socket; // one socket used for lobby packets
    // timer keeps hosted lobbies visible while players are searching
    QTimer *Lobby_Announce_Timer;
    Shared_Elements *Shared_Group; // shared state object
    QObject *Game_Multiplayer_Lobby_Group; // lobby ui controller
    QList<QPair<QHostAddress, quint16> > Connected_Client_Endpoints; // host remembers clients here
};

#endif // GAME_NETWORKING_ELEMENTS_H
