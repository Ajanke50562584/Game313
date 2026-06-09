#ifndef OBJECT_PHYSICS_BODY_H
#define OBJECT_PHYSICS_BODY_H


class Object_Physics_Body
{

public:
    Object_Physics_Body();

    float Get_X_Pos() const;
    float Get_Y_Pos() const;

    float Get_XV() const;
    float Get_YV() const;

    bool Get_Is_Grounded() const;

    float Get_Width() const;
    float Get_Height() const;

    float Get_Left() const;
    float Get_Right() const;
    float Get_Top() const;
    float Get_Bottom() const;

    void Apply_Gravity(float Gravity, float Delta_Time);

    void Update_Pos(float Delta_Time);

    void Set_Pos(float x, float y);
    void Set_Size(float Width, float Height);

    void Set_Velocity(float Vx, float Vy);
    void Set_Vx(float Vx);
    void Set_Vy(float Vy);

    void Set_Grounded_State(bool Define_Is_Grounded);

    void Jump(float Jump_Force);
    float Base_Gravity = 1000.0f;
    void Set_Grav_Scale(float New_Grav);
    void Set_Max_Fall(float New_Max_Fall);

    void Update(float Delta_Time);

    bool Is_Intersecting(float Rect_X,float Rect_Y, float Rect_Width,float Rect_Height) const;
    void Handle_Collision(float Platform_X, float Platform_Y, float Platform_Width, float Platform_Height);



private:
    // initialize the physics elements

    float X_Pos = 0.0f;
    float Y_Pos = 0.0f;

    float Width = 0.0f;
    float Height = 0.0f;

    float X_Velocity = 0.0f;
    float Y_Velocity = 0.0f;

    bool Is_Grounded = 0;

    float Grav_Scale=1.0f;
    float Max_Fall_Speed = 1200.0f;

};

#endif // OBJECT_PHYSICS_BODY_H
