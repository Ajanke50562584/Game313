#include "level.h"
#include <QDebug>

level::level()
{
    Tile_Size = 64; // keep a veelvoud van 32 asb

    Finish_Pos = QPointF(35 * Tile_Size, 6 * Tile_Size);
    Powerup_Spawns.clear();
    Debuff_Spawns.clear();
    Enemy_Spawns.clear();
    Hazard_Spawns.clear();

    // grid blocks not pixels!!
    Grid_Width = 40;
    Grid_Height = 20;
    //deafualt spawn point, can be changed per level later though
    Player_Spawn = QPointF(2 * Tile_Size, 6 * Tile_Size);


}

void level::Load_Tutorial_Level()
{
    QStringList Rows;

    Rows << "................................................................"
         << "................................................................"
         << "................................................................"
         << "........................................................F......."
         << "......................................................#####....."
         << "................................................#####..........."
         << ".............................................##................."
         << "..........................................#####................."
         << "....................................#####......................."
         << "..............................#####........E...................."
         << "..S....P..--------....#####.....D..............................."
         << "####################....########################################";

    Load_From_Ascii_Map(Rows);
}

void level::Load_Multiplayer_Level()
{
    QStringList Rows = Build_Test_Course();
    Load_From_Ascii_Map(Rows);
}

QPointF level::Grid_To_World(int gridX, int gridY) const
{
    return QPointF(gridX * Tile_Size, gridY * Tile_Size);
}

level::Map_Chunk level::Make_Chunk(const QString &name, const QString &difficulty, const QStringList &rows) const
{
    Map_Chunk Chunk;
    Chunk.Name = name;
    Chunk.Difficulty = difficulty;
    Chunk.Rows = rows;
    return Chunk;
}

QStringList level::Join_Chunks(const QVector<Map_Chunk> &chunks) const
{
    QStringList Final_Rows;

    if(chunks.size() == 0)
        return Final_Rows;

    int Chunk_Height = chunks[0].Rows.size();

    for(int y = 0; y < Chunk_Height; y++)
    {
        QString Row = "";

        for(int i = 0; i < chunks.size(); i++)
        {
            if(y < chunks[i].Rows.size())
                Row += chunks[i].Rows[y];
        }

        Final_Rows.append(Row);
    }

    return Final_Rows;
}

QStringList level::Build_Test_Course() const
{
    QVector<Map_Chunk> Chunks;

    QStringList Start_Rows;
    Start_Rows << "........................"
               << "........................"
               << "........................"
               << "........................"
               << "........................"
               << "..............#####....."
               << "............#####......."
               << "..S.---................."
               << "########################"
               << "........................"
               << "........................"
               << "........................";

    QStringList Easy_Rows;
    Easy_Rows << "........................"
              << "........................"
              << "........................"
              << "........................"
              << "...............D........."
              << "........................"
              << "..........P............."
              << ".........###............"
              << "########......##########"
              << "........................"
              << "........................"
              << "........................";

    QStringList Medium_Rows;
    Medium_Rows << "........................"
                << "........................"
                << "........................"
                << "........................"
                << "........................"
                << "............###........."
                << "............---...D....."
                << ".................###...."
                << "###########....#########"
                << "........................"
                << "........................"
                << "........................";

    QStringList Finish_Rows;
    Finish_Rows << "........................"
                << "........................"
                << "........................"
                << "........................"
                << "........................"
                << "........................"
                << "....................F..."
                << ".................######."
                << "########################"
                << "........................"
                << "........................"
                << "........................";

    Chunks.append(Make_Chunk("START_01", "start", Start_Rows));
    Chunks.append(Make_Chunk("EASY_01", "easy", Easy_Rows));
    Chunks.append(Make_Chunk("MEDIUM_01", "medium", Medium_Rows));
    Chunks.append(Make_Chunk("FINISH_01", "finish", Finish_Rows));

    return Join_Chunks(Chunks);
}

void level::Load_From_Ascii_Map(QStringList rows)
{
    //Clear all the vectors
    Platform_List.clear();
    Powerup_Spawns.clear();
    Debuff_Spawns.clear();
    Enemy_Spawns.clear();
    Hazard_Spawns.clear();

    // Set Grid Height and Width
    Grid_Height = rows.size();

    Grid_Width = 0;
    for(int i = 0; i < rows.size(); i++)
    {
        if(rows[i].length() > Grid_Width)// Take the length of the longest row
        {
            Grid_Width = rows[i].length();
        }
    }

    for(int y = 0; y<rows.size(); y++)
    {
        for(int x = 0 ; x <rows[y].length(); x++)
        {
            QChar Tile_Char = rows[y][x];
            QPointF World_Pos = Grid_To_World(x, y);

            if(Tile_Char == '#')
            {
                Platform_List.append(platform(x, y, 1, 1, 0, Tile_Size));
            }
            else if(Tile_Char == '-')
            {
                Platform_List.append(platform((float)(x * Tile_Size),
                                              (float)(y * Tile_Size),
                                              (float)Tile_Size,
                                              (float)(Tile_Size / 2),
                                              1,
                                              Tile_Size));
            }
            else if(Tile_Char == '_')
            {
                Platform_List.append(platform((float)(x * Tile_Size),
                                              (float)(y * Tile_Size + Tile_Size / 2),
                                              (float)Tile_Size,
                                              (float)(Tile_Size / 2),
                                              2,
                                              Tile_Size));
            }
            else if(Tile_Char == 'S')
            {
                Player_Spawn = World_Pos;
            }
            else if(Tile_Char == 'F')
            {
                Finish_Pos = World_Pos;
            }
            else if(Tile_Char == 'P')
            {
                Powerup_Spawns.append(World_Pos);
            }
            else if(Tile_Char == 'D')
            {
                Debuff_Spawns.append(World_Pos);
            }
            else if(Tile_Char == 'E')
            {
                Enemy_Spawns.append(World_Pos);
            }
            else if(Tile_Char == '^')
            {
                Hazard_Spawns.append(World_Pos);
            }
            else if(Tile_Char == '.')
            {
                // empty tile
            }
            else
            {
               qDebug()<< "Not a recognized tile" << Tile_Char;
            }
        }
    }
}

void level::Draw_Platforms(QPainter &painter,float cameraX,float cameraY,float scale,float viewOffsetX,float viewOffsetY) const
{
    // Loop through every platform in the level.
    for (int i = 0; i < Platform_List.size(); i++)
    {
        Platform_List[i].Draw(painter, cameraX, cameraY, scale, viewOffsetX, viewOffsetY);
    }

    float Screen_Size = Tile_Size * scale;

    // draw finish marker
    float Screen_X = viewOffsetX + (Finish_Pos.x() - cameraX) * scale;
    float Screen_Y = viewOffsetY + (Finish_Pos.y() - cameraY) * scale;
    painter.fillRect(QRectF(Screen_X, Screen_Y, Screen_Size, Screen_Size), QColor(0, 255, 0));// green

    // draw powerup spawns
    for(int i = 0; i < Powerup_Spawns.size(); i++)
    {
        Screen_X = viewOffsetX + (Powerup_Spawns[i].x() - cameraX) * scale;
        Screen_Y = viewOffsetY + (Powerup_Spawns[i].y() - cameraY) * scale;
        painter.fillRect(QRectF(Screen_X, Screen_Y, Screen_Size, Screen_Size), QColor(0, 120, 255));// blue
    }

    // draw debuff spawns
    for(int i = 0; i < Debuff_Spawns.size(); i++)
    {
        Screen_X = viewOffsetX + (Debuff_Spawns[i].x() - cameraX) * scale;
        Screen_Y = viewOffsetY + (Debuff_Spawns[i].y() - cameraY) * scale;
        painter.fillRect(QRectF(Screen_X, Screen_Y, Screen_Size, Screen_Size), QColor(180, 0, 255));//purple
    }

    // draw enemy spawns
    for(int i = 0; i < Enemy_Spawns.size(); i++)
    {
        Screen_X = viewOffsetX + (Enemy_Spawns[i].x() - cameraX) * scale;
        Screen_Y = viewOffsetY + (Enemy_Spawns[i].y() - cameraY) * scale;
        painter.fillRect(QRectF(Screen_X, Screen_Y, Screen_Size, Screen_Size), QColor(255, 0, 0));//Red
    }

    // draw hazard spawns
    for(int i = 0; i < Hazard_Spawns.size(); i++)
    {
        Screen_X = viewOffsetX + (Hazard_Spawns[i].x() - cameraX) * scale;
        Screen_Y = viewOffsetY + (Hazard_Spawns[i].y() - cameraY) * scale;
        painter.fillRect(QRectF(Screen_X, Screen_Y, Screen_Size, Screen_Size), QColor(255, 120, 0)); //Hazards
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

bool level::Is_Rect_Colliding_With_Platform(float x, float y, float w, float h) const
{
    QRectF Test_Rect(x, y, w, h);

    for(int i = 0; i < Platform_List.size(); i++)
    {
        if(Test_Rect.intersects(Platform_List[i].Get_World_Rect()))
            return true;
    }

    return false;
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

QPointF level::Get_Finish_Pos() const
{
    return Finish_Pos;
}

QVector<QPointF> level::Get_Powerup_Spawns() const
{
    return Powerup_Spawns;
}

QVector<QPointF> level::Get_Debuff_Spawns() const
{
    return Debuff_Spawns;
}

QVector<QPointF> level::Get_Enemy_Spawns() const
{
    return Enemy_Spawns;
}

QVector<QPointF> level::Get_Hazard_Spawns() const
{
    return Hazard_Spawns;
}

int level::Get_Tile_Size() const
{
    return Tile_Size;
}
