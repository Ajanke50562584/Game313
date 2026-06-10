#ifndef PLAYER_H
#define PLAYER_H

#include "object_physics_body.h" // player uses this for movement/collision

#include <QObject> // player can be a qt object
#include <QImage> // sprite sheets
#include <QPainter> // draws player
#include <QPointF> // spawn points

class Player : public QObject // local player character and animation brain
{
    Q_OBJECT // Signals and slots
public:
    explicit Player(QObject *parent = nullptr); // construct player

    int Get_Player_ID()const ; // multiplayer id
    int Get_Player_Lives()const ; // remaining lives
    bool Get_Is_Ready()const ; // ready state from lobby
    float Get_Player_Width()const ; // stored width
    float Get_Player_Height()const ; // stored height
    float Get_Player_World_X() const; // x from physics body
    float Get_Player_World_Y() const; // y from physics body
    float Get_Player_World_Size() const; // draw size in world coords

    Object_Physics_Body& Get_Player_Physics_Body();// getter for physics body info


    void Set_Player_Width(float Width); // set stored width
    void Set_Player_Height(float Height); // set stored height
    void Load_Story_Mode_Images(); // load all sprite sheets from resources
    void Set_Player_Spawn(QPointF Spawn_Point); // place player at start point
    void Draw_Story_Mode_Player(QPainter &painter,float cameraX,float cameraY,float scale,float viewOffsetX,float viewOffsetY); // draw current animation frame
    void Update_Story_Mode_Player(float dt); // update movement, physics, and animation
    void Handle_Story_Mode_Floor(float platformY); // backup floor check
    void Set_Move_Left_Pressed(bool pressed); // input from main window
    void Set_Move_Right_Pressed(bool pressed); // input from main window
    void Set_Move_Crouch_Pressed(bool pressed); // input from main window
    void Set_Can_Stand_Up(bool canStandUp); // prevents uncrouching into roof
    void Set_Move_Climb_Pressed(bool pressed); // input from main window
    void Set_Move_Jump_Pressed(bool pressed); // input from main window
    void Set_Run_Pressed(bool pressed); // shift run input

public slots:
    void Set_Player_Lives(int Lives); // update lives
    void Set_Player_Ready(bool Ready); // update lobby ready
    void Set_Player_Model(int Player_Model); // selected cat model

private:
    QImage Player_Idle_Image; // idle sprite sheet
    QImage Player_Walk_Image; // walking right sheet
    QImage Player_Walk_Left_Image; // walking left sheet
    QImage Player_Run_Image; // running right sheet
    QImage Player_Run_Left_Image; // running left sheet
    QImage Player_Jump_Image; // jumping right sheet
    QImage Player_Jump_Left_Image; // jumping left sheet
    QImage Player_Crouch_Image; // crouch right sheet
    QImage Player_Crouch_Left_Image; // crouch left sheet
    QImage Player_Climb_Image; // climb sheet

    QImage *Current_Player_Image = nullptr; // currently selected animation sheet
    // sprite sheet math info
    enum Player_Animation_State
    {
        Player_Idle,
        Player_Walk_Left,
        Player_Walk_Right,
        Player_Run_Left,
        Player_Run_Right,
        Player_Jump,
        Player_Jump_Left,
        Player_Crouch,
        Player_Crouch_Left,
        Player_Climb
    };

    Player_Animation_State Current_Animation = Player_Idle; // animation being shown now
    Player_Animation_State Previous_Animation = Player_Idle; // used to reset frames properly

    int Player_Frame = 0; // current sprite frame
    int Animation_Tick = 0; // counts update ticks for animation speed
    int Animation_Speed = 6; // lower is faster animation
    int Player_Frame_Count = 8; // fallback number of frames
    int Player_Frame_W = 32; // source frame width
    int Player_Frame_H = 32; // source frame height
    float Player_World_X = 150; // player pos in world
    float Player_World_Y = 350; // player pos in world

    bool Move_Left_Pressed = false; // left input held
    bool Move_Right_Pressed = false; // right input held
    bool Move_Crouch_Pressed = false; // crouch input held
    bool Run_Pressed = false; // shift held for running
    bool Can_Stand_Up = true; // roof check
    bool Move_Climb_Pressed = false; // climb input held
    bool Move_Jump_Pressed = false; // jump input held
    bool Player_Spawned = false; // if player has been spawned
    bool Frame_Hold = false; // holds jump frame while in air
    bool Player_Crouched = false; // true when crouch hitbox active
    int Climb_Phase = 0; // tracks climb animation phase
    // 0 = start ; 1 = climbin ; 2 = ending
    bool Player_Climbing = false; // currently in climb animation
    bool Player_Reached_Top = false; // for ending climb animation
    float Player_Vx = 0; // horizontal speed
    float Player_Vy = 0; // vertical speed

    float Gravity = 2400.0f; // cat falls fast because drama
    bool Player_Grounded = false; // can jump when true

    int Player_Lives = 9; // default lives
    int Selected_Player_Model = 1; // selected character
    float Player_Height = 0.0f; // stored height
    float Player_Width = 0.0f; // stored width

    int Player_Id = 0; // network/player id
    bool Is_Local_Player = 1; // true for this machine's player
    bool Is_Ready = 0; // ready status

    Object_Physics_Body Player_Physics_Body;// If make player add a physics body


};

#endif // PLAYER_H
