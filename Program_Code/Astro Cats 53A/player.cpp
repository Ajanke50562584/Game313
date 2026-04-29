#include "player.h"

Player::Player(QObject *parrent)
    :QObject(parrent)
{

}

float Player::Get_Player_Width() const
{
    return Player_Width;
}

float Player::Get_Player_Height() const
{
    return Player_Height;
}

Object_Physics_Body& Player::Get_Player_Physics_Body()
{
    return Player_Physics_Body;
}

void Player::Set_Player_Width(float Width)
{
    Player_Width = Width;
}

void Player::Set_Player_Height(float Height)
{
    Player_Height = Height;
}

void Player::Set_Player_Lives(int Lives)
{
    Player_Lives = Lives;
}

void Player::Set_Player_Ready(bool Ready)
{
    Is_Ready = Ready;
}

void Player::Set_Player_Model(int Player_Model)
{
    Selected_Player_Model = Player_Model;
}

