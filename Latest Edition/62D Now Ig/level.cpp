#include "level.h"

level::level()
{
    Tile_Size = 64; // keep a veelvoud van 32 asb
    // grid blocks not pixels!!
    Grid_Width = 40;
    Grid_Height = 20;
    //deafualt spawn point, can be changed per level later though
    Player_Spawn = QPointF(2 * Tile_Size, 6 * Tile_Size);
}

void level::Load_Tutorial_Level()
{
    Platform_List.clear();

    Grid_Width = 40;
    Grid_Height = 20;
    Player_Spawn = QPointF(2 * Tile_Size, 6 * Tile_Size);
    // Add static platforms in format grid column , row , width in blocks, height in blocks
    Platform_List.append(platform(2, 8, 5, 1, 0, Tile_Size));
    Platform_List.append(platform(9, 7, 5, 1, 0, Tile_Size));
    Platform_List.append(platform(16, 8, 5, 1, 0, Tile_Size));
    Platform_List.append(platform(0, 10, 25, 1, 0, Tile_Size));
}

void level::Draw_Platforms(QPainter &painter,float cameraX,float cameraY,float scale,float viewOffsetX,float viewOffsetY) const
{
    // Loop through every platform in the level.
    for (int i = 0; i < Platform_List.size(); i++)
    {
        Platform_List[i].Draw(painter, cameraX, cameraY, scale, viewOffsetX, viewOffsetY);
    }
}

void level::Handle_Player_Collision(Object_Physics_Body &playerBody) const
{
    // Loop through every platform in the level.
    for (int i = 0; i < Platform_List.size(); i++)
    {
        //fisika
        Platform_List[i].Handle_Player_Collision(playerBody);
    }
}

const QVector<platform>& level::Get_Platforms() const
{
    return Platform_List;
}

float level::Get_Level_Width() const
{
    return Grid_Width * Tile_Size;
}

float level::Get_Level_Height() const
{
    return Grid_Height * Tile_Size;
}

QPointF level::Get_Player_Spawn() const
{
    return Player_Spawn;
}

int level::Get_Tile_Size() const
{
    return Tile_Size;
}
