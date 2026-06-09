#ifndef PLATFORM_H
#define PLATFORM_H

#include <QPainter>
#include <QRectF>
#include <QColor>
#include <QPixmap>

#include "object_physics_body.h"

class platform
{
public:
    platform();
    platform(int gridX, int gridY, int gridW, int gridH, int tileType, int tileSize);
    // Draws this platform as separate square blocks
    void Draw(QPainter &painter,float cameraX,float cameraY,float scale,float viewOffsetX,float viewOffsetY) const;
    // Returns this platform's collision rectangle in world coordinates (for interaction)
    QRectF Get_World_Rect() const;

    int Get_Grid_X() const;
    int Get_Grid_Y() const;
    int Get_Grid_W() const;
    int Get_Grid_H() const;
    int Get_Type() const;
    void Handle_Player_Collision(Object_Physics_Body &playerBody) const;

    QPixmap Tile_Image;

private:
    // start stuff
    int Grid_X;
    int Grid_Y;
    int Grid_W;
    int Grid_H;
    int Tile_Type;
    int Tile_Size;
    Object_Physics_Body Platform_Body;
};

#endif // PLATFORMS_H
