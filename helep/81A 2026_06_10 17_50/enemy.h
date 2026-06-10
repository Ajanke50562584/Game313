#ifndef ENEMY_H
#define ENEMY_H

#include <QPointF>
#include <QPainter>
#include <QRectF>
#include <QImage>

#include "player.h"
#include "object_physics_body.h"

class enemy
{
public:
    enemy();

    enum Enemy_Type
        {
        Big,
        Little,
        Slow,
        Fast,
        Strong,
        Weak,
        Flying
        };

    enemy(Enemy_Type type);

    void Load_Images();
    void Set_Spawn(QPointF spawn);
    void Update(float dt, const Player &player);
    void Draw(QPainter &painter, float cameraX, float cameraY, float scale, float viewOffsetX, float viewOffsetY);
    void Take_Damage(int amount);
    bool Is_Dead() const;

    //hitbox logic
    QRectF Get_Hurtbox() const;
    QRectF Get_Attack_Hitbox() const;
    Object_Physics_Body& Get_Physics_Body();

private:
   Enemy_Type Type;
   Object_Physics_Body Enemy_Physics_Body;

   int Health;
   int Damage;
   float Speed;
   float Detection_Range;
   float Attack_Range;

   bool Facing_Left;
   bool Attacking;
   bool Attack_Has_Hit;

   QImage Idle_Image;
   QImage Walk_Image;
   QImage Attack_Image;
   QImage Hurt_Image;

   int Frame;
   int Animation_Tick;
};

#endif // ENEMY_H
