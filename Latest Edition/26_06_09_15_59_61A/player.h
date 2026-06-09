#ifndef PLAYER_H
#define PLAYER_H

#include "object_physics_body.h"

#include <QObject>
#include <QImage>
#include <QPainter>
#include <QPointF>

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
    float Get_Player_World_X() const;
    float Get_Player_World_Y() const;
    float Get_Player_World_Size() const;

    Object_Physics_Body& Get_Player_Physics_Body();// getter for physics body info


    void Set_Player_Width(float Width);
    void Set_Player_Height(float Height);
    void Load_Story_Mode_Images();
    void Set_Player_Spawn(QPointF Spawn_Point);
    void Draw_Story_Mode_Player(QPainter &painter,float cameraX,float cameraY,float scale,float viewOffsetX,float viewOffsetY);
    void Update_Story_Mode_Player(float dt);
    void Handle_Story_Mode_Floor(float platformY);
    void Set_Move_Left_Pressed(bool pressed);
    void Set_Move_Right_Pressed(bool pressed);
    void Set_Move_Crouch_Pressed(bool pressed);
    void Set_Move_Climb_Pressed(bool pressed);
    void Set_Move_Jump_Pressed(bool pressed);

public slots:
    void Set_Player_Lives(int Lives);
    void Set_Player_Ready(bool Ready);
    void Set_Player_Model(int Player_Model);

private:
    //creating the player
    QImage Player_Idle_Image;
    QImage Player_Walk_Image;
    QImage Player_Walk_Left_Image;
    QImage Player_Jump_Image;
    QImage Player_Jump_Left_Image;
    QImage Player_Crouch_Image;
    QImage Player_Crouch_Left_Image;
    QImage Player_Climb_Image;

    QImage *Current_Player_Image = nullptr;
    //sprite sheet math info
    enum Player_Animation_State
    {
        Player_Idle,
        Player_Walk_Left,
        Player_Walk_Right,
        Player_Jump,
        Player_Jump_Left,
        Player_Crouch,
        Player_Crouch_Left,
        Player_Climb
    };

    Player_Animation_State Current_Animation = Player_Idle;
    Player_Animation_State Previous_Animation = Player_Idle;

    int Player_Frame = 0;
    int Animation_Tick = 0;
    int Animation_Speed = 6;
    int Player_Frame_Count = 8;
    int Player_Frame_W = 32;
    int Player_Frame_H = 32;
    //player pos ito world pos
    float Player_World_X = 150;
    float Player_World_Y = 350;

    //for moving the player
    bool Move_Left_Pressed = false;
    bool Move_Right_Pressed = false;
    bool Move_Crouch_Pressed = false;
    bool Move_Climb_Pressed = false;
    bool Move_Jump_Pressed = false;
    bool Player_Spawned = false;
    bool Frame_Hold = false;
    bool Player_Crouched = false;
    int Climb_Phase = 0;
    // 0 = start ; 1 = climbin ; 2 = ending
    bool Player_Climbing = false;
    bool Player_Reached_Top = false;
    //Player velocities
    float Player_Vx = 0;
    float Player_Vy = 0;

    float Gravity = 2400.0f;
    bool Player_Grounded = false;

    int Player_Lives = 9;
    int Selected_Player_Model = 1;
    float Player_Height = 0.0f;
    float Player_Width = 0.0f;

    int Player_Id = 0;
    bool Is_Local_Player = 1;
    bool Is_Ready = 0;

    Object_Physics_Body Player_Physics_Body;// If make player add a physics body


};

#endif // PLAYER_H
