#ifndef STORY_MODE_TUTORIAL_H
#define STORY_MODE_TUTORIAL_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>
#include <QKeyEvent>
#include <QList>

#include "object_physics_body.h"

class Story_Mode_Tutorial : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit Story_Mode_Tutorial(QWidget *parent = nullptr);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    //creating the player
    QImage Player_Sheet;
    Object_Physics_Body Player_Body;

    int Player_Frame = 0;
    int Player_Frame_Count = 8;

    int Player_Frame_W = 32;
    int Player_Frame_H = 32;


    // This is a camera variableused to scroolll, basically follow the lil guy on the screen at all times
    float Camera_X = 0;
    float Camera_Y = 0;

    QTimer *Game_Timer;

    //moving the player
    QTimer *Game_Loop_Timer;

    bool Move_Left_Pressed = 0;
    bool Move_Right_Pressed = 0;
    bool Move_Crouch_Pressed = 0;
    bool Move_Climb_Pressed = 0;
    bool Move_Jump_Pressed = 0;
    bool Player_Spawned = 0;

    QImage Background_Image;

    int Get_Player_Draw_Size() const;
    void Draw_Rect(float x, float y, float w, float h);

    //this struct is used to build the platforms

    struct Platform_Tile
    {
        float X;
        float Y;
        float W;
        float H;
        int Tile_Type; // platform, left edge, middle, right edge

        Object_Physics_Body Platform_Body;
    };

    QList<Platform_Tile> Platform_Tiles; //keeps track of all the platforms in the tut window rn



private slots:
    void Update_Game();


};

#endif // STORY_MODE_TUTORIAL_H
