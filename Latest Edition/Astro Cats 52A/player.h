#ifndef PLAYER_H
#define PLAYER_H

#include "object_physics_body.h"

#include <QObject>

class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(QObject *parent = nullptr);

    int Get_Player_ID()const ;
    int Get_Player_Lives()const ;
    bool Get_Is_Ready()const ;
    float Get_Player_Width()const ;
    float Get_Player_Height()const ;

    Object_Physics_Body& Get_Player_Physics_Body();


    void Set_Player_Width(float Width);
    void Set_Player_Height(float Height);

public slots:
    void Set_Player_Lives(int Lives);
    void Set_Player_Ready(bool Ready);
    void Set_Player_Model(int Player_Model);

private:

    int Player_Lives = 9;
    int Selected_Player_Model = 1;
    float Player_Height = 0.0f;
    float Player_Width = 0.0f;

    int Player_Id = 0;
    bool Is_Local_Player = 1;
    bool Is_Ready = 0;

    Object_Physics_Body Player_Physics_Body;


};

#endif // PLAYER_H
