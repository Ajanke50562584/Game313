#ifndef PLATFORM_H
#define PLATFORM_H

#include <QPainter> // draws the tile
#include <QRectF> // world collision rect
#include <QColor> // colours if needed
#include <QPixmap> // platform image

#include "object_physics_body.h" // platform can collide with physics body

class platform // one solid bit of terrain
{
public:
    platform(); // default single tile
    platform(int gridX, int gridY, int gridW, int gridH, int tileType, int tileSize); // build from map grid coords
    platform(float worldX, float worldY, float worldW, float worldH, int tileType, int tileSize); // build from exact world coords
    void Draw(QPainter &painter,float cameraX,float cameraY,float scale,float viewOffsetX,float viewOffsetY) const; // Draws this platform on the camera
    QRectF Get_World_Rect() const; // Returns this platform's collision rectangle in world coordinates

    int Get_Grid_X() const; // grid x pos
    int Get_Grid_Y() const; // grid y pos
    int Get_Grid_W() const; // grid width
    int Get_Grid_H() const; // grid height
    int Get_Type() const; // tile type for special blocks
    void Handle_Player_Collision(Object_Physics_Body &playerBody) const; // resolves player against this tile

    QPixmap Tile_Image; // image used to draw the tile

private:

    int Grid_X; // tile x on ascii map
    int Grid_Y; // tile y on ascii map
    int Grid_W; // width in grid tiles
    int Grid_H; // height in grid tiles
    int Tile_Type; // normal, half, etc
    int Tile_Size; // size of one tile

    float World_X; // real x in level
    float World_Y; // real y in level
    float World_W; // real width
    float World_H; // real height

    Object_Physics_Body Platform_Body; // physics body for this platform
};

#endif // PLATFORMS_H
