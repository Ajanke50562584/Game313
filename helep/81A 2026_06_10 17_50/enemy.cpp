#include "enemy.h"

static const char *ENEMY_BIG_IDLE_PATH = ":/Images/Enemies/Big_Idle.png";
static const char *ENEMY_BIG_ATTACK_PATH = ":/Images/Enemies/Big_Attack1.png";
static const char *ENEMY_BIG_HURT_PATH = ":/Images/Enemies/Big_Hit_Death.png";

static const char *ENEMY_BAT_IDLE_PATH = ":/Images/Enemies/Bat_Idle-Fly.png";
static const char *ENEMY_BAT_WALK_PATH = ":/Images/Enemies/Bat_Fly.png";
static const char *ENEMY_BAT_ATTACK_PATH = ":/Images/Enemies/Bat_Attack.png";
static const char *ENEMY_BAT_HURT_PATH = ":/Images/Enemies/Bat_Death.png";

static const char *ENEMY_EYEBALL_IDLE_PATH = ":/Images/Enemies/EyeBall_Idle.png";
static const char *ENEMY_EYEBALL_WALK_PATH = ":/Images/Enemies/EyeBall_Walking.png";
static const char *ENEMY_EYEBALL_ATTACK_PATH = ":/Images/Enemies/EyeBall_Attack.png";
static const char *ENEMY_EYEBALL_HURT_PATH = ":/Images/Enemies/EyeBall_Death.png";

static const float ENEMY_WORLD_SIZE = 96.0f;
static const int ENEMY_FRAME_W = 32;
static const int ENEMY_FRAME_H = 32;
static const int ENEMY_ANIMATION_SPEED = 6;

enemy::enemy()
{
    Type = Little;
    Health = 1;
    Damage = 1;
    Speed = 100;
    Detection_Range = 350;
    Attack_Range = 60;
    Facing_Left = false;
    Attacking = false;
    Attack_Has_Hit = false;
    Frame = 0;
    Animation_Tick = 0;

    Load_Images();
    Enemy_Physics_Body.Base_Gravity = 2400.0f;
    Enemy_Physics_Body.Set_Grav_Scale(1.0f);
    Enemy_Physics_Body.Set_Max_Fall(1400.0f);
    Enemy_Physics_Body.Set_Size(ENEMY_WORLD_SIZE * 0.55f, ENEMY_WORLD_SIZE * 0.85f);
}

enemy::enemy(Enemy_Type type)
{
    Type = type;
    Detection_Range = 350;
    Attack_Range = 60;
    Facing_Left = false;
    Attacking = false;
    Attack_Has_Hit = false;
    Frame = 0;
    Animation_Tick = 0;

    switch (type)
    {
    case Big:
        Health = 2;
        Damage = 2;
        Speed = 70;
        Detection_Range = 300;
        Attack_Range = 75;
        break;
    case Little:
        Health = 1;
        Damage = 1;
        Speed = 130;
        break;
    case Slow:
        Health = 1;
        Damage = 1;
        Speed = 60;
        break;
    case Fast:
        Health = 1;
        Damage = 1;
        Speed = 200;
        Detection_Range = 450;
        break;
    case Strong:
        Health = 2;
        Damage = 3;
        Speed = 90;
        break;
    case Weak:
        Health = 1;
        Damage = 1;
        Speed = 110;
        break;
    case Flying:
        Health = 1;
        Damage = 2;
        Speed = 160;
        Detection_Range = 500;
        Attack_Range = 70;
        break;
    }

    Load_Images();
    Enemy_Physics_Body.Base_Gravity = 2400.0f;
    Enemy_Physics_Body.Set_Grav_Scale(Type == Flying ? 0.0f : 1.0f);
    Enemy_Physics_Body.Set_Max_Fall(1400.0f);
    Enemy_Physics_Body.Set_Size(ENEMY_WORLD_SIZE * 0.55f, ENEMY_WORLD_SIZE * 0.85f);
}

void enemy::Load_Images()
{
    if (Type == Big)
    {
        Idle_Image = QImage(ENEMY_BIG_IDLE_PATH);
        Walk_Image = QImage(ENEMY_BIG_IDLE_PATH);
        Attack_Image = QImage(ENEMY_BIG_ATTACK_PATH);
        Hurt_Image = QImage(ENEMY_BIG_HURT_PATH);
    }
    else if (Type == Flying)
    {
        Idle_Image = QImage(ENEMY_BAT_IDLE_PATH);
        Walk_Image = QImage(ENEMY_BAT_WALK_PATH);
        Attack_Image = QImage(ENEMY_BAT_ATTACK_PATH);
        Hurt_Image = QImage(ENEMY_BAT_HURT_PATH);
    }
    else
    {
        Idle_Image = QImage(ENEMY_EYEBALL_IDLE_PATH);
        Walk_Image = QImage(ENEMY_EYEBALL_WALK_PATH);
        Attack_Image = QImage(ENEMY_EYEBALL_ATTACK_PATH);
        Hurt_Image = QImage(ENEMY_EYEBALL_HURT_PATH);
    }
}

void enemy::Set_Spawn(QPointF spawn)
{
    Enemy_Physics_Body.Set_Pos(spawn.x(), spawn.y());
}

void enemy::Update(float dt, const Player &player)
{
    if (Is_Dead() == true)
    {
        Enemy_Physics_Body.Set_Velocity(0, Enemy_Physics_Body.Get_YV());
        return;
    }

    float Player_X = player.Get_Player_World_X();
    float Enemy_X = Enemy_Physics_Body.Get_X_Pos();
    float Distance_X = Player_X - Enemy_X;
    float Absolute_Distance_X = Distance_X;

    if (Absolute_Distance_X < 0)
    {
        Absolute_Distance_X = -Absolute_Distance_X;
    }

    Attacking = false;

    if (Absolute_Distance_X <= Attack_Range)
    {
        Attacking = true;
        Enemy_Physics_Body.Set_Vx(0);
    }
    else if (Absolute_Distance_X <= Detection_Range)
    {
        if (Distance_X < 0)
        {
            Facing_Left = true;
            Enemy_Physics_Body.Set_Vx(-Speed);
        }
        else
        {
            Facing_Left = false;
            Enemy_Physics_Body.Set_Vx(Speed);
        }
    }
    else
    {
        Enemy_Physics_Body.Set_Vx(0);
    }

    Animation_Tick++;

    if (Animation_Tick >= ENEMY_ANIMATION_SPEED)
    {
        Animation_Tick = 0;
        Frame++;
    }

    Enemy_Physics_Body.Set_Grounded_State(false);
    Enemy_Physics_Body.Update(dt);
}

void enemy::Draw(QPainter &painter, float cameraX, float cameraY, float scale, float viewOffsetX, float viewOffsetY)
{
    QImage *Current_Image = &Idle_Image;

    if (Is_Dead() == true)
    {
        Current_Image = &Hurt_Image;
    }
    else if (Attacking == true)
    {
        Current_Image = &Attack_Image;
    }
    else if (Enemy_Physics_Body.Get_XV() != 0)
    {
        Current_Image = &Walk_Image;
    }

    if (Current_Image == nullptr || Current_Image->isNull())
    {
        return;
    }

    int Frame_Count = Current_Image->width() / ENEMY_FRAME_W;

    if (Frame_Count <= 0)
    {
        return;
    }

    int Source_X = (Frame % Frame_Count) * ENEMY_FRAME_W;
    QRect Source_Rect(Source_X, 0, ENEMY_FRAME_W, ENEMY_FRAME_H);

    QRectF Target_Rect(viewOffsetX + (Enemy_Physics_Body.Get_X_Pos() - cameraX - (ENEMY_WORLD_SIZE * 0.20f)) * scale,
                       viewOffsetY + (Enemy_Physics_Body.Get_Bottom() - cameraY - ENEMY_WORLD_SIZE) * scale,
                       ENEMY_WORLD_SIZE * scale,
                       ENEMY_WORLD_SIZE * scale);

    painter.drawImage(Target_Rect, *Current_Image, Source_Rect);
}

void enemy::Take_Damage(int amount)
{
    Health = Health - amount;

    if (Health < 0)
    {
        Health = 0;
    }
}

bool enemy::Is_Dead() const
{
    return Health <= 0;
}

QRectF enemy::Get_Hurtbox() const
{
    return QRectF(Enemy_Physics_Body.Get_Left(),
                  Enemy_Physics_Body.Get_Top(),
                  Enemy_Physics_Body.Get_Width(),
                  Enemy_Physics_Body.Get_Height());
}

QRectF enemy::Get_Attack_Hitbox() const
{
    float Attack_Width = Attack_Range;
    float Attack_Height = ENEMY_WORLD_SIZE * 0.45f;
    float Attack_X;

    if (Facing_Left == true)
    {
        Attack_X = Enemy_Physics_Body.Get_Left() - Attack_Width;
    }
    else
    {
        Attack_X = Enemy_Physics_Body.Get_Right();
    }

    float Attack_Y = Enemy_Physics_Body.Get_Top() + ENEMY_WORLD_SIZE * 0.20f;

    return QRectF(Attack_X, Attack_Y, Attack_Width, Attack_Height);
}

Object_Physics_Body& enemy::Get_Physics_Body()
{
    return Enemy_Physics_Body;
}
