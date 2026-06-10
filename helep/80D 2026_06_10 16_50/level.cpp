#include "level.h"
#include <QDebug>
#include <QPair>
#include <QRandomGenerator>
#include <QTextStream>

level::level()
{
    Tile_Size = 64; // keep a veelvoud van 32 asb

    Finish_Pos = QPointF(35 * Tile_Size, 6 * Tile_Size);
    Powerup_Spawns.clear();
    Debuff_Spawns.clear();
    Enemy_Spawns.clear();
    Hazard_Spawns.clear();
    Climb_Spawns.clear();

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

void level::Load_Single_Player_Level_Number(int levelNumber)
{
    QStringList Rows = Build_Single_Player_Course(levelNumber);

    if(Rows.isEmpty())
        Rows = Build_Fallback_Course();

    Load_From_Ascii_Map(Rows);
}

void level::Load_Multiplayer_Level()
{
    Load_Multiplayer_Level("Medium");
}

void level::Load_Multiplayer_Level(const QString &difficulty)
{
    QStringList Rows = Build_Course_For_Difficulty(difficulty);

    if(Rows.isEmpty())
        Rows = Build_Fallback_Course();

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

level::Map_Chunk level::Load_Chunk_From_File(const QString &filePath) const
{
    Map_Chunk Map_Segment;
    Map_Segment.Chunk_File_Path= filePath;

    QFile File(filePath);

    if(!File.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Could not open chunk file:" << File.errorString();
        return Map_Segment;
    }

    QTextStream In(&File);

    while(!In.atEnd())
    {
        QString Line = In.readLine();
        if(Line.startsWith("# Name:"))
        {
            Map_Segment.Name = Line.mid(QString("# Name:").length()).trimmed();
        }
        else if(Line.startsWith("# Type:"))
        {
            Map_Segment.Type = Line.mid(QString("# Type:").length()).trimmed();
        }
        else if(Line.startsWith("# Difficulty:"))
        {
            Map_Segment.Difficulty = Line.mid(QString("# Difficulty:").length()).trimmed();
        }
        else if(!Line.trimmed().isEmpty())
        {
            Map_Segment.Rows.append(Line);
        }
    }

    File.close();
    return Map_Segment;

}

QStringList level::Join_Chunks(const QVector<Map_Chunk> &chunks) const
{
    QStringList Final_Rows;

    if(chunks.size() == 0)
        return Final_Rows;

    int Chunk_Height = 0;

    for(int i = 0; i < chunks.size(); i++)
    {
        if(chunks[i].Rows.size() > Chunk_Height)
            Chunk_Height = chunks[i].Rows.size();
    }

    for(int y = 0; y < Chunk_Height; y++)
    {
        QString Row = "";

        for(int i = 0; i < chunks.size(); i++)
        {
            int Chunk_Width = 24;

            if(!chunks[i].Rows.isEmpty())
                Chunk_Width = chunks[i].Rows[0].length();

            if(y < chunks[i].Rows.size())
            {
                QString Chunk_Row = chunks[i].Rows[y];

                while(Chunk_Row.length() < Chunk_Width)
                    Chunk_Row += ".";

                Row += Chunk_Row;
            }
            else
            {
                Row += QString(Chunk_Width, QChar('.'));
            }
        }

        Final_Rows.append(Row);
    }

    return Final_Rows;
}

QStringList level::Build_Test_Course() const
{
    return Build_Course_For_Difficulty("Medium");
}

QStringList level::Build_Fallback_Course() const
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
                << "..................D....."
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

level::Map_Chunk level::Load_First_Working_Chunk(const QStringList &filePaths) const
{
    for(int i = 0; i < filePaths.size(); i++)
    {
        if(!QFile::exists(filePaths[i]))
            continue;

        Map_Chunk Chunk = Load_Chunk_From_File(filePaths[i]);

        if(!Chunk.Rows.isEmpty())
            return Chunk;
    }

    return Map_Chunk();
}

void level::Add_Random_Chunks(QVector<Map_Chunk> &courseChunks, const QStringList &filePaths, int amount) const
{
    if(filePaths.isEmpty())
        return;

    int Added = 0;
    int Attempts = 0;
    const int Max_Attempts = qMax(amount * filePaths.size() * 2, amount);

    while(Added < amount && Attempts < Max_Attempts)
    {
        Attempts++;
        const int Random_Index = QRandomGenerator::global()->bounded(filePaths.size());

        if(!QFile::exists(filePaths[Random_Index]))
            continue;

        Map_Chunk Chunk = Load_Chunk_From_File(filePaths[Random_Index]);

        if(!Chunk.Rows.isEmpty())
        {
            courseChunks.append(Chunk);
            Added++;
        }
    }
}

QStringList level::Build_Course_For_Difficulty(const QString &difficulty) const
{
    QString Clean_Difficulty = difficulty.trimmed();

    if(Clean_Difficulty != "Easy" && Clean_Difficulty != "Medium" && Clean_Difficulty != "Hard")
        Clean_Difficulty = "Medium";

    const QStringList Start_Files =
    {
        ":/Spawn_Tile/Spawn_01.txt",
        "Spawn_01.txt"
    };

    const QStringList Finish_Files =
    {
        ":/Finish_Tile/Finish_01.txt",
        "Finish_01.txt"
    };

    const QStringList Easy_Files =
    {
        ":/Map_Easy_Txt/Easy_01_IntroEnemyStep.txt",
        ":/Map_Easy_Txt/Easy_02_CrouchEnemy.txt",
        ":/Map_Easy_Txt/Easy_03_ClimbEnemy.txt",
        ":/Map_Easy_Txt/Easy_04_SafeHazardEnemy.txt",
        ":/Map_Easy_Txt/Easy_05_BackstepEnemy.txt",
        "Easy_01_IntroEnemyStep.txt",
        "Easy_02_CrouchEnemy.txt",
        "Easy_03_ClimbEnemy.txt",
        "Easy_04_SafeHazardEnemy.txt",
        "Easy_05_BackstepEnemy.txt"
    };

    const QStringList Medium_Files =
    {
        ":/Map_Medium_Txt/Medium_01_EnemySwitchback.txt",
        ":/Map_Medium_Txt/Medium_02_HazardEnemyClimb.txt",
        ":/Map_Medium_Txt/Medium_03_CrouchClimbEnemy.txt",
        ":/Map_Medium_Txt/Medium_04_BacktrackColumnEnemy.txt",
        ":/Map_Medium_Txt/Medium_05_EnemyGapRun.txt",
        "Medium_01_EnemySwitchback.txt",
        "Medium_02_HazardEnemyClimb.txt",
        "Medium_03_CrouchClimbEnemy.txt",
        "Medium_04_BacktrackColumnEnemy.txt",
        "Medium_05_EnemyGapRun.txt"
    };

    const QStringList Hard_Files =
    {
        ":/Map_Hard_Txt/Hard_01_BacktrackTowerEnemy.txt",
        ":/Map_Hard_Txt/Hard_02_DownUpEnemyTrap.txt",
        ":/Map_Hard_Txt/Hard_03_ColumnPressureEnemy.txt",
        ":/Map_Hard_Txt/Hard_04_HighRoadEnemy.txt",
        ":/Map_Hard_Txt/Hard_05_FinalLoopEnemy.txt",
        "Hard_01_BacktrackTowerEnemy.txt",
        "Hard_02_DownUpEnemyTrap.txt",
        "Hard_03_ColumnPressureEnemy.txt",
        "Hard_04_HighRoadEnemy.txt",
        "Hard_05_FinalLoopEnemy.txt"
    };

    QVector<Map_Chunk> Course_Chunks;

    Map_Chunk Start_Chunk = Load_First_Working_Chunk(Start_Files);
    Map_Chunk Finish_Chunk = Load_First_Working_Chunk(Finish_Files);

    if(Start_Chunk.Rows.isEmpty() || Finish_Chunk.Rows.isEmpty())
        return Build_Fallback_Course();

    Course_Chunks.append(Start_Chunk);

    if(Clean_Difficulty == "Easy")
    {
        Add_Random_Chunks(Course_Chunks, Easy_Files, 3);
        Add_Random_Chunks(Course_Chunks, Medium_Files, 1);
    }
    else if(Clean_Difficulty == "Medium")
    {
        Add_Random_Chunks(Course_Chunks, Easy_Files, 2);
        Add_Random_Chunks(Course_Chunks, Medium_Files, 3);
        Add_Random_Chunks(Course_Chunks, Hard_Files, 1);
    }
    else
    {
        Add_Random_Chunks(Course_Chunks, Easy_Files, 1);
        Add_Random_Chunks(Course_Chunks, Medium_Files, 3);
        Add_Random_Chunks(Course_Chunks, Hard_Files, 4);
    }

    Course_Chunks.append(Finish_Chunk);

    return Join_Chunks(Course_Chunks);
}

QStringList level::Build_Single_Player_Course(int levelNumber) const
{
    QVector<Map_Chunk> Course_Chunks;

    auto Chunk_Paths = [](const QString &prefix, const QString &fileName)
    {
        QStringList Paths;
        Paths << prefix + "/" + fileName
              << fileName;
        return Paths;
    };

    auto Load_Named_Chunk = [this, Chunk_Paths](const QString &prefix, const QString &fileName)
    {
        return Load_First_Working_Chunk(Chunk_Paths(prefix, fileName));
    };

    Map_Chunk Start_Chunk = Load_Named_Chunk(":/Spawn_Tile", "Spawn_01.txt");
    Map_Chunk Finish_Chunk = Load_Named_Chunk(":/Finish_Tile", "Finish_01.txt");

    if(Start_Chunk.Rows.isEmpty() || Finish_Chunk.Rows.isEmpty())
        return Build_Fallback_Course();

    Course_Chunks.append(Start_Chunk);

    int Level = qBound(1, levelNumber, 10);

    QVector<QPair<QString, QString> > Fixed_Order;

    if(Level == 1)
    {
        Fixed_Order.append(qMakePair(QString(":/Map_Easy_Txt"), QString("Easy_01_IntroEnemyStep.txt")));
        Fixed_Order.append(qMakePair(QString(":/Map_Easy_Txt"), QString("Easy_03_ClimbEnemy.txt")));
    }
    else if(Level == 2)
    {
        Fixed_Order.append(qMakePair(QString(":/Map_Easy_Txt"), QString("Easy_02_CrouchEnemy.txt")));
        Fixed_Order.append(qMakePair(QString(":/Map_Easy_Txt"), QString("Easy_04_SafeHazardEnemy.txt")));
        Fixed_Order.append(qMakePair(QString(":/Map_Easy_Txt"), QString("Easy_05_BackstepEnemy.txt")));
    }
    else if(Level == 3)
    {
        Fixed_Order.append(qMakePair(QString(":/Map_Easy_Txt"), QString("Easy_03_ClimbEnemy.txt")));
        Fixed_Order.append(qMakePair(QString(":/Map_Medium_Txt"), QString("Medium_01_EnemySwitchback.txt")));
        Fixed_Order.append(qMakePair(QString(":/Map_Easy_Txt"), QString("Easy_04_SafeHazardEnemy.txt")));
    }
    else if(Level == 4)
    {
        Fixed_Order.append(qMakePair(QString(":/Map_Easy_Txt"), QString("Easy_05_BackstepEnemy.txt")));
        Fixed_Order.append(qMakePair(QString(":/Map_Medium_Txt"), QString("Medium_03_CrouchClimbEnemy.txt")));
        Fixed_Order.append(qMakePair(QString(":/Map_Medium_Txt"), QString("Medium_05_EnemyGapRun.txt")));
    }
    else if(Level == 5)
    {
        Fixed_Order.append(qMakePair(QString(":/Map_Medium_Txt"), QString("Medium_01_EnemySwitchback.txt")));
        Fixed_Order.append(qMakePair(QString(":/Map_Medium_Txt"), QString("Medium_02_HazardEnemyClimb.txt")));
        Fixed_Order.append(qMakePair(QString(":/Map_Medium_Txt"), QString("Medium_04_BacktrackColumnEnemy.txt")));
    }
    else if(Level == 6)
    {
        Fixed_Order.append(qMakePair(QString(":/Map_Easy_Txt"), QString("Easy_02_CrouchEnemy.txt")));
        Fixed_Order.append(qMakePair(QString(":/Map_Medium_Txt"), QString("Medium_04_BacktrackColumnEnemy.txt")));
        Fixed_Order.append(qMakePair(QString(":/Map_Hard_Txt"), QString("Hard_02_DownUpEnemyTrap.txt")));
    }
    else if(Level == 7)
    {
        Fixed_Order.append(qMakePair(QString(":/Map_Medium_Txt"), QString("Medium_02_HazardEnemyClimb.txt")));
        Fixed_Order.append(qMakePair(QString(":/Map_Hard_Txt"), QString("Hard_01_BacktrackTowerEnemy.txt")));
        Fixed_Order.append(qMakePair(QString(":/Map_Medium_Txt"), QString("Medium_05_EnemyGapRun.txt")));
        Fixed_Order.append(qMakePair(QString(":/Map_Hard_Txt"), QString("Hard_03_ColumnPressureEnemy.txt")));
    }
    else if(Level == 8)
    {
        Fixed_Order.append(qMakePair(QString(":/Map_Medium_Txt"), QString("Medium_03_CrouchClimbEnemy.txt")));
        Fixed_Order.append(qMakePair(QString(":/Map_Hard_Txt"), QString("Hard_02_DownUpEnemyTrap.txt")));
        Fixed_Order.append(qMakePair(QString(":/Map_Hard_Txt"), QString("Hard_04_HighRoadEnemy.txt")));
        Fixed_Order.append(qMakePair(QString(":/Map_Medium_Txt"), QString("Medium_04_BacktrackColumnEnemy.txt")));
    }
    else if(Level == 9)
    {
        Fixed_Order.append(qMakePair(QString(":/Map_Hard_Txt"), QString("Hard_01_BacktrackTowerEnemy.txt")));
        Fixed_Order.append(qMakePair(QString(":/Map_Hard_Txt"), QString("Hard_03_ColumnPressureEnemy.txt")));
        Fixed_Order.append(qMakePair(QString(":/Map_Hard_Txt"), QString("Hard_04_HighRoadEnemy.txt")));
        Fixed_Order.append(qMakePair(QString(":/Map_Hard_Txt"), QString("Hard_05_FinalLoopEnemy.txt")));
    }
    else
    {
        Fixed_Order.append(qMakePair(QString(":/Map_Medium_Txt"), QString("Medium_05_EnemyGapRun.txt")));
        Fixed_Order.append(qMakePair(QString(":/Map_Hard_Txt"), QString("Hard_02_DownUpEnemyTrap.txt")));
        Fixed_Order.append(qMakePair(QString(":/Map_Hard_Txt"), QString("Hard_01_BacktrackTowerEnemy.txt")));
        Fixed_Order.append(qMakePair(QString(":/Map_Hard_Txt"), QString("Hard_03_ColumnPressureEnemy.txt")));
        Fixed_Order.append(qMakePair(QString(":/Map_Hard_Txt"), QString("Hard_05_FinalLoopEnemy.txt")));
    }

    for(int i = 0; i < Fixed_Order.size(); i++)
    {
        Map_Chunk Chunk = Load_Named_Chunk(Fixed_Order[i].first, Fixed_Order[i].second);

        if(!Chunk.Rows.isEmpty())
            Course_Chunks.append(Chunk);
    }

    Course_Chunks.append(Finish_Chunk);

    return Join_Chunks(Course_Chunks);
}

void level::Load_From_Ascii_Map(QStringList rows)
{
    //Clear all the vectors
    Platform_List.clear();
    Powerup_Spawns.clear();
    Debuff_Spawns.clear();
    Enemy_Spawns.clear();
    Hazard_Spawns.clear();
    Climb_Spawns.clear();

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
            else if(Tile_Char == 'K')
            {
                Climb_Spawns.append(World_Pos);
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

    // draw climbable blocks
    for(int i = 0; i < Climb_Spawns.size(); i++)
    {
        Screen_X = viewOffsetX + (Climb_Spawns[i].x() - cameraX) * scale;
        Screen_Y = viewOffsetY + (Climb_Spawns[i].y() - cameraY) * scale;
        painter.fillRect(QRectF(Screen_X, Screen_Y, Screen_Size, Screen_Size), QColor(255, 230, 80)); //Climb
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

QVector<QPointF> level::Get_Climb_Spawns() const
{
    return Climb_Spawns;
}

int level::Get_Tile_Size() const
{
    return Tile_Size;
}
