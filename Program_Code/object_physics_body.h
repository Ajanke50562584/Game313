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

        void Apply_Gravity(float Gravity, float Delta_Time);

        void Update_Pos(float Delta_Time);

        void Set_Pos(float x, float y);
        void Set_Velocity(float Vx, float Vy);

        void Set_Vx(float Vx);
        void Set_Vy(float Vy);

        void Set_Grounded_State(bool Define_Is_Grounded);

        void Jump(float Jump_Force);



private:
    // initialize the physics elements

    float X_Pos = 0.0f;
    float Y_Pos = 0.0f;

    float X_Velocity = 0.0f;
    float Y_Velocity = 0.0f;

    bool Is_Grounded = 0;

};

#endif // OBJECT_PHYSICS_BODY_H
