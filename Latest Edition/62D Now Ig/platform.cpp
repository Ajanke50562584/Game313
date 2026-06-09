#include "platform.h"

platform::platform()
{
    Grid_X = 0;
    Grid_Y = 0;
    Grid_W = 1;
    Grid_H = 1;
    Tile_Type = 0;
    Tile_Size = 64;

    Tile_Image = QPixmap(":/Images/Tiles/Plat_Tile.png");
    //fisika
    Platform_Body.Set_Pos(Grid_X * Tile_Size, Grid_Y * Tile_Size);
    //fisika
    Platform_Body.Set_Size(Grid_W * Tile_Size, Grid_H * Tile_Size);
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

    Tile_Image = QPixmap(":/Images/Tiles/Plat_Tile.png");
    //fisika
    Platform_Body.Set_Pos(Grid_X * Tile_Size, Grid_Y * Tile_Size);
    //fisika
    Platform_Body.Set_Size(Grid_W * Tile_Size, Grid_H * Tile_Size);
}

void platform::Draw(QPainter &painter,float cameraX,float cameraY,float scale,float viewOffsetX,float viewOffsetY) const
{
    for (int row = 0; row < Grid_H; row++) //platform row
    {
        for (int col = 0; col < Grid_W; col++) //columns
        {
            // convert from grid to world coordinates
            float World_X = (Grid_X + col) * Tile_Size;
            float World_Y = (Grid_Y + row) * Tile_Size;
            //how it appears on screen in verhouding met camera
            float Screen_X = viewOffsetX + (World_X - cameraX) * scale;
            float Screen_Y = viewOffsetY + (World_Y - cameraY) * scale;
            // Calculate the square tile size on screen.
            float Screen_Size = Tile_Size * scale;
            // Create the rectangle where this tile will be drawn.
            QRectF Tile_Rect(Screen_X, Screen_Y, Screen_Size, Screen_Size);
            // Draw a temporary grey block for now.
            painter.drawPixmap(Tile_Rect, Tile_Image, Tile_Image.rect());
        }
    }
}

QRectF platform::Get_World_Rect() const
{
    // Convert the platform grid coordinates to world
    float World_X = Grid_X * Tile_Size;
    float World_Y = Grid_Y * Tile_Size;
    float World_W = Grid_W * Tile_Size;
    float World_H = Grid_H * Tile_Size;
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
    playerBody.Handle_Collision(Platform_Body.Get_X_Pos(),
                                Platform_Body.Get_Y_Pos(),
                                Platform_Body.Get_Width(),
                                Platform_Body.Get_Height());
}
