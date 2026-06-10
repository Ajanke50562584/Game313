#ifndef LEVEL_H
#define LEVEL_H

#include <QVector>
#include <QPainter>
#include <QRectF>
#include <QPointF>
#include <QColor>
#include <QString>
#include <QStringList>

#include "platform.h"
#include "object_physics_body.h"

class level
{
public:
    level();

    // Loads the tutorial level layout
    void Load_Tutorial_Level();
    void Load_Multiplayer_Level();
    // Draws all platforms in the level
    void Draw_Platforms(QPainter &painter,float cameraX,float cameraY,float scale,float viewOffsetX,float viewOffsetY) const;

    void Handle_Player_Collision(Object_Physics_Body &playerBody) const;
    bool Is_Rect_Colliding_With_Platform(float x, float y, float w, float h) const;

    const QVector<platform>& Get_Platforms() const;
    QPointF Get_Player_Spawn() const;
    QPointF Get_Finish_Pos() const;
    QVector<QPointF> Get_Powerup_Spawns() const;
    QVector<QPointF> Get_Debuff_Spawns() const;
    QVector<QPointF> Get_Enemy_Spawns() const;
    QVector<QPointF> Get_Hazard_Spawns() const;
    int Get_Tile_Size() const;
    float Get_Level_Width() const;
    float Get_Level_Height() const;

   QPointF Grid_To_World(int gridX, int gridY) const;
   void Load_From_Ascii_Map(QStringList rows);

   struct Map_Chunk
   {
       QString Name;
       QString Difficulty;
       QStringList Rows;
   };


private:
    int Tile_Size;
    int Grid_Width;
    int Grid_Height;

    QPointF Player_Spawn;
    QPointF Finish_Pos;
    QVector<platform> Platform_List;
    QVector<QPointF> Powerup_Spawns;
    QVector<QPointF> Debuff_Spawns;
    QVector<QPointF> Enemy_Spawns;
    QVector<QPointF> Hazard_Spawns;

    QStringList Build_Test_Course() const;
    QStringList Join_Chunks(const QVector<Map_Chunk> &chunks) const;
    Map_Chunk Make_Chunk(const QString &name, const QString &difficulty, const QStringList &rows) const;


};

#endif // LEVEL_H
