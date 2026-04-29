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

void Object_Physics_Body::Apply_Gravity(float Gravity, float Delta_Time)
{
    Y_Velocity += Gravity*Delta_Time;
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
