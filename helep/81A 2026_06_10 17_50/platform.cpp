#include "platform.h"

platform::platform()
{
    Grid_X = 0;
    Grid_Y = 0;
    Grid_W = 1;
    Grid_H = 1;
    Tile_Type = 0;
    Tile_Size = 64;

    World_X = Grid_X * Tile_Size;
    World_Y = Grid_Y * Tile_Size;
    World_W = Grid_W * Tile_Size;
    World_H = Grid_H * Tile_Size;

    Tile_Image = QPixmap(":/Images/Tiles/Plat_Tile.png");
    //fisika
    Platform_Body.Set_Pos(World_X, World_Y);
    //fisika
    Platform_Body.Set_Size(World_W, World_H);
}

platform::platform(int gridX, int gridY, int gridW, int gridH, int tileType, int tileSize)
{
    //store whats needed to make it right
    Grid_X = gridX;
    Grid_Y = gridY;
    Grid_W = gridW;
    Grid_H = gridH;
    Tile_Type = tileType;
    Tile_Size = tileSize;

    World_X = Grid_X * Tile_Size;
    World_Y = Grid_Y * Tile_Size;
    World_W = Grid_W * Tile_Size;
    World_H = Grid_H * Tile_Size;

    Tile_Image = QPixmap(":/Images/Tiles/Plat_Tile.png");
    //fisika
    Platform_Body.Set_Pos(World_X, World_Y);
    //fisika
    Platform_Body.Set_Size(World_W, World_H);
}

platform::platform(float worldX, float worldY, float worldW, float worldH, int tileType, int tileSize)
{
    Grid_X = worldX / tileSize;
    Grid_Y = worldY / tileSize;
    Grid_W = 1;
    Grid_H = 1;
    Tile_Type = tileType;
    Tile_Size = tileSize;

    World_X = worldX;
    World_Y = worldY;
    World_W = worldW;
    World_H = worldH;

    Tile_Image = QPixmap(":/Images/Tiles/Plat_Tile.png");

    //fisika
    Platform_Body.Set_Pos(World_X, World_Y);
    //fisika
    Platform_Body.Set_Size(World_W, World_H);
}

void platform::Draw(QPainter &painter,float cameraX,float cameraY,float scale,float viewOffsetX,float viewOffsetY) const
{
    //how it appears on screen in verhouding met camera
    float Screen_X = viewOffsetX + (World_X - cameraX) * scale;
    float Screen_Y = viewOffsetY + (World_Y - cameraY) * scale;
    float Screen_W = World_W * scale;
    float Screen_H = World_H * scale;

    QRectF Tile_Rect(Screen_X, Screen_Y, Screen_W, Screen_H);

    // Draw the tile image into this platform rectangle
    painter.drawPixmap(Tile_Rect, Tile_Image, Tile_Image.rect());
}

QRectF platform::Get_World_Rect() const
{
    return QRectF(World_X, World_Y, World_W, World_H); // full coordinates
}

int platform::Get_Grid_X() const
{
    return Grid_X;
}

int platform::Get_Grid_Y() const
{
    return Grid_Y;
}

int platform::Get_Grid_W() const
{
    return Grid_W;
}

int platform::Get_Grid_H() const
{
    return Grid_H;
}

int platform::Get_Type() const
{
    return Tile_Type;
}

void platform::Handle_Player_Collision(Object_Physics_Body &playerBody) const
{
    //fisika
    playerBody.Handle_Collision(World_X,World_Y, World_W,World_H);
}
