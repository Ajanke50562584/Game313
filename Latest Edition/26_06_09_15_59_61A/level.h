#ifndef LEVEL_H
#define LEVEL_H

#include <QVector>
#include <QPainter>
#include <QRectF>
#include <QPointF>
#include <QColor>

#include "platform.h"
#include "object_physics_body.h"

class level
{
public:
    level();

    // Loads the tutorial level layout
    void Load_Tutorial_Level();
    // Draws all platforms in the level
    void Draw_Platforms(QPainter &painter,float cameraX,float cameraY,float scale,float viewOffsetX,float viewOffsetY) const;
    void Handle_Player_Collision(Object_Physics_Body &playerBody) const;
    const QVector<platform>& Get_Platforms() const;
    float Get_Level_Width() const;
    float Get_Level_Height() const;
    QPointF Get_Player_Spawn() const;
    int Get_Tile_Size() const;

private:
    int Tile_Size;
    int Grid_Width;
    int Grid_Height;

    QPointF Player_Spawn;
    QVector<platform> Platform_List;
};

#endif // LEVEL_H
