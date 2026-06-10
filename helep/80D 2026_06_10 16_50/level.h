#ifndef LEVEL_H
#define LEVEL_H

#include <QVector> // holds platforms and spawn points
#include <QPainter> // draws level tiles
#include <QRectF> // collision rectangles
#include <QPointF> // world positions
#include <QString> // chunk names
#include <QStringList> // ascii map rows
#include <QFile>
#include <QFileDevice>

#include "platform.h"
#include "object_physics_body.h" // player collision checks

class level
{
public:
    level();

    // Setup Map
    void Load_Tutorial_Level();
    void Load_Single_Player_Level_Number(int levelNumber);
    void Load_Multiplayer_Level();
    void Load_Multiplayer_Level(const QString &difficulty);
    void Draw_Platforms(QPainter &painter,float cameraX,float cameraY,float scale,float viewOffsetX,float viewOffsetY) const;
    //Collision handling
    void Handle_Player_Collision(Object_Physics_Body &playerBody) const;
    bool Is_Rect_Colliding_With_Platform(float x, float y, float w, float h) const;

    const QVector<platform>& Get_Platforms() const; // gives access to terrain list
    QPointF Get_Player_Spawn() const;
    QPointF Get_Finish_Pos() const; // green
    QVector<QPointF> Get_Powerup_Spawns() const; // blue
    QVector<QPointF> Get_Debuff_Spawns() const; // purple
    QVector<QPointF> Get_Enemy_Spawns() const; // red
    QVector<QPointF> Get_Hazard_Spawns() const; // orange
    QVector<QPointF> Get_Climb_Spawns() const;
    int Get_Tile_Size() const; // pixel size of one grid tile
    float Get_Level_Width() const; // world width in pixels
    float Get_Level_Height() const; // world height in pixels

   QPointF Grid_To_World(int gridX, int gridY) const; // convert map blocks to real position
   void Load_From_Ascii_Map(QStringList rows); // Map character setup

   struct Map_Chunk
   {
       QString Name;
       QString Type;
       QString Difficulty;
       QStringList Rows;
       QString Chunk_File_Path;
   };


private:
    int Tile_Size;
    int Grid_Width; // longest row length
    int Grid_Height; // number of map rows

    QPointF Player_Spawn; // S
    QPointF Finish_Pos; // F
    QVector<platform> Platform_List; // #
    QVector<QPointF> Powerup_Spawns; // P
    QVector<QPointF> Debuff_Spawns; // D
    QVector<QPointF> Enemy_Spawns; // E
    QVector<QPointF> Hazard_Spawns; // ^
    QVector<QPointF> Climb_Spawns; // K

    QStringList Build_Test_Course() const; // builds current multiplayer course
    QStringList Build_Course_For_Difficulty(const QString &difficulty) const;
    QStringList Build_Single_Player_Course(int levelNumber) const;
    QStringList Build_Fallback_Course() const;
    QStringList Join_Chunks(const QVector<Map_Chunk> &chunks) const; // glues chunks side by side
    Map_Chunk Make_Chunk(const QString &name, const QString &difficulty, const QStringList &rows) const;

    Map_Chunk Load_Chunk_From_File(const QString &filePath) const;
    Map_Chunk Load_First_Working_Chunk(const QStringList &filePaths) const;
    void Add_Random_Chunks(QVector<Map_Chunk> &courseChunks, const QStringList &filePaths, int amount) const;


};

#endif // LEVEL_H
