#include "player.h"

// These image paths come directly from Resources.qrc.
static const char *PLAYER_WALK_PATH = ":/Images/Characters/Walking_Gray.png";
static const char *PLAYER_WALK_LEFT_PATH = ":/Images/Characters/Walking_Left_Gray.png";
static const char *PLAYER_IDLE_PATH   = ":/Images/Characters/Idle_Gray.png";
static const char *PLAYER_JUMP_PATH   = ":/Images/Characters/Jumping_Gray.png";
static const char *PLAYER_JUMP_LEFT_PATH = ":/Images/Characters/Jumping_Left_Gray.png";
static const char *PLAYER_CROUCH_PATH = ":/Images/Characters/Crouching_Gray.png";
static const char *PLAYER_CROUCH_LEFT_PATH = ":/Images/Characters/Crouching_Left_Gray.png";
static const char *PLAYER_CLIMB_PATH  = ":/Images/Characters/Climbing_Gray.png";
// Player size in tutorial world coordinates before scaling to the current window.
static const float STORY_MODE_PLAYER_WORLD_SIZE = 96.0f;

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

float Player::Get_Player_World_X() const
{
    //fisika
    return Player_Physics_Body.Get_X_Pos();
}

float Player::Get_Player_World_Y() const
{
    //fisika
    return Player_Physics_Body.Get_Y_Pos();
}

float Player::Get_Player_World_Size() const
{
    return STORY_MODE_PLAYER_WORLD_SIZE;
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

void Player::Load_Story_Mode_Images()
{
    //set up for different images at different movement stages
    Player_Idle_Image = QImage(PLAYER_IDLE_PATH);
    Player_Walk_Image = QImage(PLAYER_WALK_PATH);
    Player_Walk_Left_Image = QImage(PLAYER_WALK_LEFT_PATH);
    Player_Jump_Image = QImage(PLAYER_JUMP_PATH);
    Player_Jump_Left_Image = QImage(PLAYER_JUMP_LEFT_PATH);
    Player_Crouch_Image = QImage(PLAYER_CROUCH_PATH);
    Player_Crouch_Left_Image = QImage(PLAYER_CROUCH_LEFT_PATH);
    Player_Climb_Image = QImage(PLAYER_CLIMB_PATH);

    Current_Player_Image = &Player_Idle_Image;
}

void Player::Set_Player_Spawn(QPointF Spawn_Point)
{
    Player_World_X = Spawn_Point.x();
    Player_World_Y = Spawn_Point.y();
    //fisika
    Player_Physics_Body.Set_Pos(Player_World_X, Player_World_Y);
    //fisika
    Player_Physics_Body.Base_Gravity = Gravity;
    //fisika
    Player_Physics_Body.Set_Grav_Scale(1.0f);
    //fisika
    Player_Physics_Body.Set_Max_Fall(1400.0f);
}

void Player::Draw_Story_Mode_Player(QPainter &painter,float cameraX,float cameraY,float scale,float viewOffsetX,float viewOffsetY)
{
    {
        if (Current_Player_Image == nullptr || Current_Player_Image->isNull())
        {
            return;
        }
    }

    int Current_Frame_Count = Current_Player_Image->width() / Player_Frame_W; //split the sheet into frames

    if (Player_Frame >= Current_Frame_Count) //looping mechanism
    {
        Player_Frame = 0;
    }

    int sourceX = Player_Frame * Player_Frame_W; //finds pos in sprite sheet
    //int playerDrawSize = Get_Player_Draw_Size(); //size [player should be
    QRect sourceRect(sourceX, 0, Player_Frame_W, Player_Frame_H); //makes the rectangle for the frame (which frame in sheet it takes)
    //fisika
    QRectF targetRect(viewOffsetX + (Player_Physics_Body.Get_X_Pos() - cameraX - (STORY_MODE_PLAYER_WORLD_SIZE * 0.20f)) * scale,viewOffsetY + (Player_Physics_Body.Get_Y_Pos() - cameraY - (STORY_MODE_PLAYER_WORLD_SIZE * 0.15f)) * scale,STORY_MODE_PLAYER_WORLD_SIZE * scale,STORY_MODE_PLAYER_WORLD_SIZE * scale); //actual drawn rect on screen
    painter.drawImage(targetRect, *Current_Player_Image, sourceRect); //draw the frame in the created rect for it on screen
}

void Player::Update_Story_Mode_Player(float dt)
{
    //fisika
    Player_Physics_Body.Set_Size(STORY_MODE_PLAYER_WORLD_SIZE * 0.55f,STORY_MODE_PLAYER_WORLD_SIZE * 0.85f);

    // left / right movement
    if (Move_Left_Pressed == true)
    {
        Player_Vx = -500;
        //fisika
        Player_Physics_Body.Set_Vx(Player_Vx);

    }
    else if (Move_Right_Pressed == true)
    {
        Player_Vx = 500;
        //fisika
        Player_Physics_Body.Set_Vx(Player_Vx);

    }
    else
    {
        Player_Vx = 0;
        //fisika
        Player_Physics_Body.Set_Vx(Player_Vx);
    }

    // jump
    //fisika
    Player_Grounded = Player_Physics_Body.Get_Is_Grounded();

    if (Move_Jump_Pressed == true && Player_Grounded == true)
    {
        Player_Vy = -700;
        Player_Grounded = false;
        //fisika
        Player_Physics_Body.Jump(700);
    }

    //Tests which animation is needed for the set movement
    Previous_Animation = Current_Animation;

    //fisika
    Player_Grounded = Player_Physics_Body.Get_Is_Grounded();

    if (Player_Grounded == false)// tests if jump and jump in which dir
    {
        if (Move_Left_Pressed == true)
        {
            Current_Animation = Player_Jump_Left;
            Current_Player_Image = &Player_Jump_Left_Image;
        }
        else if (Move_Right_Pressed == true)
        {
            Current_Animation = Player_Jump;
            Current_Player_Image = &Player_Jump_Image;
        }
        else if (Previous_Animation == Player_Walk_Left || Previous_Animation == Player_Jump_Left)
        {
            Current_Animation = Player_Jump_Left;
            Current_Player_Image = &Player_Jump_Left_Image;
        }
        else
        {
            Current_Animation = Player_Jump;
            Current_Player_Image = &Player_Jump_Image;
        }
    }
    else if (Move_Crouch_Pressed == 1) //tests if busy crouching
    {
        if (Move_Left_Pressed == true || Previous_Animation == Player_Walk_Left || Previous_Animation == Player_Jump_Left || Previous_Animation == Player_Crouch_Left)
        {
            Current_Animation = Player_Crouch_Left;
            Current_Player_Image = &Player_Crouch_Left_Image;
        }
        else
        {
            Current_Animation = Player_Crouch;
            Current_Player_Image = &Player_Crouch_Image;
        }
    }
    else if (Move_Climb_Pressed == 1 || Player_Climbing == true) //tests if busy climbing
    {
        Current_Animation = Player_Climb;
        Current_Player_Image = &Player_Climb_Image;

        if (Player_Climbing == false)
        {
            Player_Climbing = true;
            Climb_Phase = 0;
            Player_Frame = 0;
            Animation_Tick = 0;
        }
    }
    else if (Move_Left_Pressed == 1)
    {
        Current_Animation = Player_Walk_Left;
        Current_Player_Image = &Player_Walk_Left_Image;
    }
    else if (Move_Right_Pressed == 1)
    {
        Current_Animation = Player_Walk_Right;
        Current_Player_Image = &Player_Walk_Image;
    }
    else
    {
        Current_Animation = Player_Idle;
        Current_Player_Image = &Player_Idle_Image;
    }

    //Starts new movement at the first frame, important for jumps and stuff but does make glitchy sometimes
    if (Current_Animation != Previous_Animation)
    {
        Animation_Tick = 0;

        if (Current_Animation == Player_Crouch)
           {
               Player_Frame = 2;
           }
           else if (Current_Animation == Player_Crouch_Left)
           {
               Player_Frame = 1;
           }
           else if (Current_Animation != Player_Climb)
           {
               Player_Frame = 0;
           }
    }

    // animation
    Animation_Tick++;

    if (Animation_Tick >= Animation_Speed)
    {
        Animation_Tick = 0;

        if (Current_Animation == Player_Jump || Current_Animation == Player_Jump_Left) // keeps jump animation in air
        {
            if (Frame_Hold == false)
            {
                Player_Frame++;

                if (Player_Frame == 3)
                {
                    Frame_Hold = true;
                }
            }
        }
        else if (Current_Animation == Player_Crouch)
        {
            // pulse between frame 3 and frame 4
            if (Player_Frame == 2)
            {
                Player_Frame = 3;
            }
            else
            {
                Player_Frame = 2;
            }
        }else if (Current_Animation == Player_Crouch_Left)
        {
            // pulse between frame 1 and frame 2
            if (Player_Frame == 1)
            {
                Player_Frame = 0;
            }
            else
            {
                Player_Frame = 1;
            }
        }
        else if (Current_Animation == Player_Climb)
        {
            if (Climb_Phase == 0)
            {
                // 0, 1, 2, 3 start climb
                Player_Frame++;

                if (Player_Frame > 3)
                {
                    Climb_Phase = 1;
                    Player_Frame = 4;
                }
            }
            else if (Climb_Phase == 1)
            {
                // 4, 5, 6, 7 actual climb
                Player_Frame++;

                if (Player_Frame > 7)
                {
                    Player_Frame = 4;
                }

                if (Player_Reached_Top == true)
                {
                    Climb_Phase = 2;
                    Player_Frame = 8;
                }
            }
            else if (Climb_Phase == 2)
            {
                // 8, 9, 10, 11 climb over ledge
                Player_Frame++;

                if (Player_Frame > 11) // done with the climbing sequence
                {
                    Player_Frame = 0;
                    Player_Climbing = false;
                    Player_Reached_Top = false;
                    Climb_Phase = 0;
                }
            }
        }
        else
        {
            Player_Frame++;

            if (Player_Frame >= Player_Frame_Count)
            {
                Player_Frame = 0;
            }
        }
    }

    //fisika
    Player_Physics_Body.Set_Grounded_State(false);
    //fisika
    Player_Physics_Body.Update(dt);
    //fisika
    Player_World_X = Player_Physics_Body.Get_X_Pos();
    //fisika
    Player_World_Y = Player_Physics_Body.Get_Y_Pos();
    //fisika
    Player_Vy = Player_Physics_Body.Get_YV();
}

void Player::Handle_Story_Mode_Floor(float platformY)
{
    //fisika
    if (Player_Physics_Body.Get_Bottom() > platformY)
    {
        // Keep the player on top of the world-space floor.
        //fisika
        Player_World_Y = platformY - Player_Physics_Body.Get_Height();
        Player_Vy = 0;
        Player_Grounded = true;
        Frame_Hold = false;
        //fisika
        Player_Physics_Body.Set_Pos(Player_Physics_Body.Get_X_Pos(), Player_World_Y);
        //fisika
        Player_Physics_Body.Set_Vy(0);
        //fisika
        Player_Physics_Body.Set_Grounded_State(true);
    }
}

void Player::Set_Move_Left_Pressed(bool pressed)
{
    Move_Left_Pressed = pressed;
}

void Player::Set_Move_Right_Pressed(bool pressed)
{
    Move_Right_Pressed = pressed;
}

void Player::Set_Move_Crouch_Pressed(bool pressed)
{
    Move_Crouch_Pressed = pressed;
}

void Player::Set_Move_Climb_Pressed(bool pressed)
{
    Move_Climb_Pressed = pressed;
}

void Player::Set_Move_Jump_Pressed(bool pressed)
{
    Move_Jump_Pressed = pressed;
}
