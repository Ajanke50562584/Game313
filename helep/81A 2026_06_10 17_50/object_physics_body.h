#ifndef OBJECT_PHYSICS_BODY_H
#define OBJECT_PHYSICS_BODY_H


class Object_Physics_Body // small reusable physics box
{

public:
    Object_Physics_Body(); // starts with empty/default physics values

    float Get_X_Pos() const; // current left x
    float Get_Y_Pos() const; // current top y

    float Get_XV() const; // horizontal velocity
    float Get_YV() const; // vertical velocity

    bool Get_Is_Grounded() const; // true if standing on something

    float Get_Width() const; // body width
    float Get_Height() const; // body height

    float Get_Left() const; // left side of box
    float Get_Right() const; // right side of box
    float Get_Top() const; // top side of box
    float Get_Bottom() const; // bottom side of box
    float Get_Previous_Top() const; // top before last update
    float Get_Previous_Bottom() const; // bottom before last update

    void Apply_Gravity(float Gravity, float Delta_Time); // add gravity to y velocity

    void Update_Pos(float Delta_Time); // move by current velocity

    void Set_Pos(float x, float y); // force position
    void Set_Size(float Width, float Height); // change collision size
    void Set_Velocity(float Vx, float Vy); // set both velocities

    void Set_Vx(float Vx); // set x velocity
    void Set_Vy(float Vy); // set y velocity

    void Set_Grounded_State(bool Define_Is_Grounded); // tells body if it landed

    void Jump(float Jump_Force); // throw body upward
    float Base_Gravity = 1000.0f; // normal falling strength
    void Set_Grav_Scale(float New_Grav); // changes gravity multiplier
    void Set_Max_Fall(float New_Max_Fall); // terminal velocity basically

    void Update(float Delta_Time); // full physics tick

    bool Is_Intersecting(float Rect_X,float Rect_Y, float Rect_Width,float Rect_Height) const; // test against another box
    void Handle_Collision(float Platform_X, float Platform_Y, float Platform_Width, float Platform_Height); // push out of platform



private:
    float X_Pos = 0.0f; // current x
    float Y_Pos = 0.0f; // current y
    float Previous_X_Pos = 0.0f; // x before move
    float Previous_Y_Pos = 0.0f; // y before move

    float Width = 0.0f; // hitbox width
    float Height = 0.0f; // hitbox height

    float X_Velocity = 0.0f; // speed sideways
    float Y_Velocity = 0.0f; // speed up/down

    bool Is_Grounded = 0; // can jump if this is true

    float Grav_Scale=1.0f; // gravity multiplier
    float Max_Fall_Speed = 1200.0f; // stop falling faster forever

};

#endif // OBJECT_PHYSICS_BODY_H
