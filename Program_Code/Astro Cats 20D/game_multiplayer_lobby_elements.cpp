#include "game_multiplayer_lobby_elements.h"

#include "shared_elements.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

Game_Multiplayer_Lobby_Elements::Game_Multiplayer_Lobby_Elements(QWidget *parent)
    : QObject(parent)
    , Shared_Group(nullptr)
{
    Lobby_Widget = new QWidget(parent);

    QVBoxLayout *Main_Layout = new QVBoxLayout(Lobby_Widget);

    Lobby_Name_Label = new QLabel("The Last Constellation Lobby", Lobby_Widget);
    Connection_Status_Label = new QLabel("Connection Status: Idle", Lobby_Widget);
    Player_Role_Label = new QLabel("Role: Offline", Lobby_Widget);
    Remote_Address_Label = new QLabel("Remote Address: Not connected", Lobby_Widget);
    Lobby_Type_Label = new QLabel("Lobby Type: Public", Lobby_Widget);
    Max_Players_Label = new QLabel("Max Players: 4", Lobby_Widget);
    Last_Event_Label = new QLabel("Last Event: No network activity yet", Lobby_Widget);

    Lobby_Name_Label->setAlignment(Qt::AlignHCenter);
    Connection_Status_Label->setAlignment(Qt::AlignHCenter);
    Player_Role_Label->setAlignment(Qt::AlignHCenter);
    Remote_Address_Label->setAlignment(Qt::AlignHCenter);
    Lobby_Type_Label->setAlignment(Qt::AlignHCenter);
    Max_Players_Label->setAlignment(Qt::AlignHCenter);
    Last_Event_Label->setAlignment(Qt::AlignHCenter);

    Lobby_Widget->setStyleSheet(
        "background-color: rgb(15, 20, 35);"
        "color: rgb(235, 235, 245);"
        "font-size: 22px;"
        "font-weight: bold;");

    Main_Layout->addStretch();
    Main_Layout->addWidget(Lobby_Name_Label);
    Main_Layout->addWidget(Connection_Status_Label);
    Main_Layout->addWidget(Player_Role_Label);
    Main_Layout->addWidget(Remote_Address_Label);
    Main_Layout->addWidget(Lobby_Type_Label);
    Main_Layout->addWidget(Max_Players_Label);
    Main_Layout->addWidget(Last_Event_Label);
    Main_Layout->addStretch();
}

void Game_Multiplayer_Lobby_Elements::Attach_Shared_Elements(Shared_Elements *sharedElements)
{
    Shared_Group = sharedElements;
    Refresh_From_Shared_Elements();
}

void Game_Multiplayer_Lobby_Elements::Refresh_From_Shared_Elements()
{
    if(Shared_Group == nullptr)
        return;

    Lobby_Name_Label->setText(Shared_Group->Get_Lobby_Name());
    Connection_Status_Label->setText("Connection Status: " + Shared_Group->Get_Connection_Status());
    Player_Role_Label->setText("Role: " + Shared_Group->Get_Player_Role());
    Remote_Address_Label->setText("Remote Address: " + Shared_Group->Get_Remote_Address());
    Lobby_Type_Label->setText(QString("Lobby Type: ") + (Shared_Group->Get_Is_Public_Lobby() ? "Public" : "Private"));
    Max_Players_Label->setText("Max Players: " + QString::number(Shared_Group->Get_Max_Players()));
    Last_Event_Label->setText("Last Event: " + Shared_Group->Get_Last_Network_Event());
}

QWidget *Game_Multiplayer_Lobby_Elements::Get_Lobby_Widget() const
{
    return Lobby_Widget;
}
