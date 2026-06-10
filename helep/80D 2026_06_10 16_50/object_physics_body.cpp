#include "object_physics_body.h"

Object_Physics_Body::Object_Physics_Body()
{

}

// every getter function known to man

float Object_Physics_Body::Get_X_Pos() const
{
    return X_Pos;
}

float Object_Physics_Body::Get_Y_Pos() const
{
    return Y_Pos;
}

float Object_Physics_Body::Get_XV() const
{
    return X_Velocity;
}

float Object_Physics_Body::Get_YV() const
{
    return Y_Velocity;
}

bool Object_Physics_Body::Get_Is_Grounded() const
{
    return Is_Grounded;
}

float Object_Physics_Body::Get_Width() const
{
    return Width;
}

float Object_Physics_Body::Get_Height() const
{
    return Height;
}

float Object_Physics_Body::Get_Left() const
{
    return X_Pos;
}

float Object_Physics_Body::Get_Right() const
{
    return X_Pos+Width;
}

float Object_Physics_Body::Get_Top() const
{
    return Y_Pos;
}

float Object_Physics_Body::Get_Bottom() const
{
    return Y_Pos+Height;
}

float Object_Physics_Body::Get_Previous_Top() const
{
    return Previous_Y_Pos;
}

float Object_Physics_Body::Get_Previous_Bottom() const
{
    return Previous_Y_Pos + Height;
}

void Object_Physics_Body::Apply_Gravity(float Gravity, float Delta_Time)
{
    Y_Velocity += Gravity*Delta_Time*Grav_Scale;

    if(Y_Velocity>Max_Fall_Speed)
        Y_Velocity = Max_Fall_Speed;
}

void Object_Physics_Body::Update_Pos(float Delta_Time)
{
    Previous_X_Pos = X_Pos;
    Previous_Y_Pos = Y_Pos;

    X_Pos += Delta_Time*X_Velocity;
    Y_Pos += Delta_Time*Y_Velocity;
}

void Object_Physics_Body::Set_Pos(float x, float y)
{
    X_Pos = x;
    Y_Pos = y;
    Previous_X_Pos = x;
    Previous_Y_Pos = y;
}

void Object_Physics_Body::Set_Size(float Width, float Height)
{
    this->Width=Width;
    this->Height=Height;
}

void Object_Physics_Body::Set_Velocity(float Vx, float Vy)
{
    X_Velocity = Vx;
    Y_Velocity = Vy;
}

void Object_Physics_Body::Set_Vx(float Vx)
{
    X_Velocity = Vx;
}

void Object_Physics_Body::Set_Vy(float Vy)
{
    Y_Velocity = Vy;
}

void Object_Physics_Body::Set_Grounded_State(bool Define_Is_Grounded)
{
    Is_Grounded = Define_Is_Grounded;
}

void Object_Physics_Body::Jump(float Jump_Force)
{
    if(Is_Grounded == 1)
    {
        Is_Grounded = 0;
        Y_Velocity = -Jump_Force;
    }
}

void Object_Physics_Body::Set_Grav_Scale(float New_Grav)
{
    Grav_Scale = New_Grav;
}

void Object_Physics_Body::Set_Max_Fall(float New_Max_Fall)
{
    Max_Fall_Speed = New_Max_Fall;
}

void Object_Physics_Body::Update(float Delta_Time)
{
    Apply_Gravity(Base_Gravity,Delta_Time);
    Update_Pos(Delta_Time);
}

bool Object_Physics_Body::Is_Intersecting(float Rect_X, float Rect_Y, float Rect_Width, float Rect_Height) const
{
    return Get_Right() > Rect_X && Get_Left() < Rect_X+Rect_Width && Get_Top() <Rect_Y+Rect_Height && Get_Bottom() > Rect_Y;
}

void Object_Physics_Body::Handle_Collision(float Platform_X,float Platform_Y,float Platform_Width,float Platform_Height)
{
    if(!Is_Intersecting(Platform_X, Platform_Y, Platform_Width, Platform_Height))
    {
        return;
    }

    const float Platform_Left = Platform_X;
    const float Platform_Right = Platform_X + Platform_Width;
    const float Platform_Bottom = Platform_Y + Platform_Height;
    const float Previous_Left = Previous_X_Pos;
    const float Previous_Right = Previous_X_Pos + Width;
    const float Previous_Top = Previous_Y_Pos;
    const float Previous_Bottom = Previous_Y_Pos + Height;
    const float Center_X = X_Pos + Width * 0.5f;

    const float Overlap_Left = Get_Left() > Platform_Left ? Get_Left() : Platform_Left;
    const float Overlap_Right = Get_Right() < Platform_Right ? Get_Right() : Platform_Right;
    const float Horizontal_Overlap = Overlap_Right - Overlap_Left;

    const float Overlap_Top = Get_Top() > Platform_Y ? Get_Top() : Platform_Y;
    const float Overlap_Bottom = Get_Bottom() < Platform_Bottom ? Get_Bottom() : Platform_Bottom;
    const float Vertical_Overlap = Overlap_Bottom - Overlap_Top;

    const bool Can_Land_On_Block = Horizontal_Overlap >= Width * 0.45f;
    const bool Can_Hit_Ceiling = Horizontal_Overlap >= Width * 0.98f &&
                                 Center_X > Platform_Left + 8.0f &&
                                 Center_X < Platform_Right - 8.0f;
    const bool Can_Hit_Horizontal = Vertical_Overlap >= Height * 0.20f;
    const float Ceiling_Hit_Allowance = 16.0f;

    const bool Hit_From_Top = Can_Land_On_Block &&
                              Previous_Bottom <= Platform_Y &&
                              Get_Bottom() >= Platform_Y &&
                              Y_Velocity >= 0;

    const bool Hit_From_Bottom = Can_Hit_Ceiling &&
                                 Previous_Top >= Platform_Bottom &&
                                 Get_Top() <= Platform_Bottom - Ceiling_Hit_Allowance &&
                                 Y_Velocity <= 0;

    const bool Hit_From_Left = Can_Hit_Horizontal &&
                               Previous_Right <= Platform_Left &&
                               Get_Right() >= Platform_Left &&
                               X_Velocity > 0;

    const bool Hit_From_Right = Can_Hit_Horizontal &&
                                Previous_Left >= Platform_Right &&
                                Get_Left() <= Platform_Right &&
                                X_Velocity < 0;

    if(Hit_From_Top)
    {
        Y_Pos = Platform_Y - Height;
        Y_Velocity = 0;
        Is_Grounded = 1;
        return;
    }

    if(Hit_From_Bottom)
    {
        Y_Pos = Platform_Bottom;
        Y_Velocity = 0;
        return;
    }

    if(Hit_From_Left)
    {
        X_Pos = Platform_Left - Width;
        X_Velocity = -250;
        return;
    }

    if(Hit_From_Right)
    {
        X_Pos = Platform_Right;
        X_Velocity = 250;
        return;
    }

    // fallback for corner overlaps; prefer the axis the player is moving through
    const float Push_Left = Get_Right() - Platform_Left;
    const float Push_Right = Platform_Right - Get_Left();
    const float Push_Up = Get_Bottom() - Platform_Y;
    const float Push_Down = Platform_Bottom - Get_Top();

    if(Can_Hit_Horizontal && X_Velocity > 0 && Push_Left <= Push_Up + 8.0f)
    {
        X_Pos = Platform_Left - Width;
        X_Velocity = -250;
        return;
    }

    if(Can_Hit_Horizontal && X_Velocity < 0 && Push_Right <= Push_Up + 8.0f)
    {
        X_Pos = Platform_Right;
        X_Velocity = 250;
        return;
    }

    if(Y_Velocity > 0 && Can_Land_On_Block)
    {
        Y_Pos = Platform_Y - Height;
        Y_Velocity = 0;
        Is_Grounded = 1;
        return;
    }

    if(Y_Velocity < 0 && Can_Hit_Ceiling && Get_Top() <= Platform_Bottom - Ceiling_Hit_Allowance)
    {
        Y_Pos = Platform_Bottom;
        Y_Velocity = 0;
        return;
    }

    if(Can_Hit_Horizontal && Push_Left < Push_Right)
    {
        X_Pos = Platform_Left - Width;
        X_Velocity = -250;
        return;
    }

    if(Can_Hit_Horizontal)
    {
        X_Pos = Platform_Right;
        X_Velocity = 250;
        return;
    }

    // if the player is moving upward and this was not a clear ceiling hit,
    // do not kill the jump because of a small corner overlap
    if(Y_Velocity < 0)
    {
        return;
    }

    // tiny overlaps should not pull the player onto or under a block
    if(!Can_Land_On_Block)
    {
        return;
    }

    // last resort for rare deep overlaps; choose the shortest way out
    float Smallest_Push = Push_Left;
    int Collision_Side = 0;

    if(Push_Right < Smallest_Push)
    {
        Smallest_Push = Push_Right;
        Collision_Side = 1;
    }

    if(Push_Up < Smallest_Push)
    {
        Smallest_Push = Push_Up;
        Collision_Side = 2;
    }

    if(Push_Down < Smallest_Push)
    {
        Collision_Side = 3;
    }

    if(Collision_Side == 0)
    {
        X_Pos = Platform_Left - Width;
        X_Velocity = -250;
    }
    else if(Collision_Side == 1)
    {
        X_Pos = Platform_Right;
        X_Velocity = 250;
    }
    else if(Collision_Side == 2)
    {
        Y_Pos = Platform_Y - Height;
        Y_Velocity = 0;
        Is_Grounded = 1;
    }
    else
    {
        Y_Pos = Platform_Bottom;
        Y_Velocity = 0;
    }
}
