#include "game_networking_elements.h"

#include "game_multiplayer_lobby_elements.h"
#include "shared_elements.h"

#include <QDebug>
#include <QUdpSocket>

Game_Networking_Elements::Game_Networking_Elements(QObject *parent)
    : QObject(parent)
    , Shared_Group(nullptr)
    , Game_Multiplayer_Lobby_Group(nullptr)
{
    Game_UDP_Socket = new QUdpSocket(this);

    connect(Game_UDP_Socket, &QUdpSocket::readyRead, this, [this]() {
        Read_Pending_Datagrams();
    });
}

void Game_Networking_Elements::Attach_Shared_Elements(Shared_Elements *sharedElements)
{
    Shared_Group = sharedElements;
}

void Game_Networking_Elements::Attach_Multiplayer_Lobby_Elements(Game_Multiplayer_Lobby_Elements *lobbyElements)
{
    Game_Multiplayer_Lobby_Group = lobbyElements;
}

bool Game_Networking_Elements::Start_Listening()
{
    const bool Listen_Was_Started = Game_UDP_Socket->bind(QHostAddress::AnyIPv4, 30033);

    if(Shared_Group != nullptr)
    {
        Shared_Group->Set_Is_Hosting(Listen_Was_Started);
        Shared_Group->Set_Is_In_Lobby(Listen_Was_Started);
        Shared_Group->Set_Player_Role(Listen_Was_Started ? "Host" : "Offline");
        Shared_Group->Set_Connection_Status(Listen_Was_Started ? "Listening for players" : "Failed to start host");
        Shared_Group->Set_Remote_Address(Listen_Was_Started ? "0.0.0.0:30033" : "Not connected");
        Shared_Group->Set_Last_Network_Event(Listen_Was_Started ? "Host is waiting for join requests" : "Bind on port 30033 failed");
    }

    Refresh_Lobby_View();
    return Listen_Was_Started;
}

void Game_Networking_Elements::Request_To_Join(const QHostAddress &address, quint16 port)
{
    if(Shared_Group != nullptr)
    {
        Shared_Group->Set_Is_Hosting(false);
        Shared_Group->Set_Is_In_Lobby(false);
        Shared_Group->Set_Player_Role("Client");
        Shared_Group->Set_Connection_Status("Sending join request");
        Shared_Group->Set_Remote_Address(address.toString() + ":" + QString::number(port));
        Shared_Group->Set_Last_Network_Event("Join request sent to host");
    }

    Send_Datagram("REQUEST_TO_JOIN", address, port);
    Refresh_Lobby_View();
}

void Game_Networking_Elements::Send_Datagram(const QString &Datagram, QHostAddress address, quint16 port)
{
    QByteArray Data = Datagram.toUtf8();
    Game_UDP_Socket->writeDatagram(Data, address, port);
}

void Game_Networking_Elements::Read_Pending_Datagrams()
{
    while(Game_UDP_Socket->hasPendingDatagrams())
    {
        QByteArray Datagram;
        Datagram.resize(Game_UDP_Socket->pendingDatagramSize());

        QHostAddress Sender_Address;
        quint16 Sender_Port;

        Game_UDP_Socket->readDatagram(Datagram.data(), Datagram.size(), &Sender_Address, &Sender_Port);

        const QString Datagram_Data = QString::fromUtf8(Datagram);

        qDebug() << "Received:" << Datagram_Data;
        qDebug() << "From IP:" << Sender_Address.toString();
        qDebug() << "From Port:" << Sender_Port;

        switch(Get_Datagram_Type(Datagram_Data))
        {
        case REQUEST_TO_JOIN:
            qDebug() << "A player wants to join";
            if(Shared_Group != nullptr)
            {
                Shared_Group->Set_Connection_Status("Player joined host");
                Shared_Group->Set_Remote_Address(Sender_Address.toString() + ":" + QString::number(Sender_Port));
                Shared_Group->Set_Last_Network_Event("Join request received and accepted");
                Shared_Group->Set_Is_In_Lobby(true);
            }
            Send_Datagram("JOIN_ACCEPTED", Sender_Address, Sender_Port);
            break;

        case JOIN_ACCEPTED:
            qDebug() << "Join request accepted";
            if(Shared_Group != nullptr)
            {
                Shared_Group->Set_Connection_Status("Connected to host");
                Shared_Group->Set_Remote_Address(Sender_Address.toString() + ":" + QString::number(Sender_Port));
                Shared_Group->Set_Last_Network_Event("Host accepted join request");
                Shared_Group->Set_Is_In_Lobby(true);
            }
            break;

        default:
            qDebug() << "Unknown datagram type";
            if(Shared_Group != nullptr)
            {
                Shared_Group->Set_Last_Network_Event("Unknown datagram received");
            }
            break;
        }

        Refresh_Lobby_View();
    }
}

Game_Networking_Elements::Datagram_Type Game_Networking_Elements::Get_Datagram_Type(const QString &Datagram)
{
    if(Datagram == "REQUEST_TO_JOIN")
        return REQUEST_TO_JOIN;

    if(Datagram == "JOIN_ACCEPTED")
        return JOIN_ACCEPTED;

    return UNKNOWN;
}

void Game_Networking_Elements::Refresh_Lobby_View()
{
    if(Game_Multiplayer_Lobby_Group != nullptr)
        Game_Multiplayer_Lobby_Group->Refresh_From_Shared_Elements();
}
