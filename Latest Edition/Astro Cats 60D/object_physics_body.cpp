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

void Object_Physics_Body::Apply_Gravity(float Gravity, float Delta_Time)
{
    Y_Velocity += Gravity*Delta_Time*Grav_Scale;

    if(Y_Velocity>Max_Fall_Speed)
        Y_Velocity = Max_Fall_Speed;
}

void Object_Physics_Body::Update_Pos(float Delta_Time)
{
    X_Pos += Delta_Time*X_Velocity;
    Y_Pos += Delta_Time*Y_Velocity;
}

void Object_Physics_Body::Set_Pos(float x, float y)
{
    X_Pos = x;
    Y_Pos = y;
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

void Object_Physics_Body::Handle_Collision(float Platform_X,
                                           float Platform_Y,
                                           float Platform_Width,
                                           float Platform_Height)
{
    if(Y_Velocity > 0 && Is_Intersecting(Platform_X, Platform_Y, Platform_Width, Platform_Height))
    {
        float Platform_Left = Platform_X;
        float Platform_Right = Platform_X + Platform_Width;

        float Overlap_Left = Get_Left() > Platform_Left ? Get_Left() : Platform_Left;
        float Overlap_Right = Get_Right() < Platform_Right ? Get_Right() : Platform_Right;

        float Horizontal_Overlap = Overlap_Right - Overlap_Left;
        float Required_Overlap = Width * 0.45f; // falls once more than 55% is off platform

        if(Horizontal_Overlap >= Required_Overlap)
        {
            Y_Pos = Platform_Y - Height;
            Y_Velocity = 0;
            Is_Grounded = 1;
        }
    }
}
