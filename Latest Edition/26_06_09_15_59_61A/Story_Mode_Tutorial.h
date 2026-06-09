#ifndef STORY_MODE_TUTORIAL_H
#define STORY_MODE_TUTORIAL_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>
#include <QKeyEvent>
// Needed because this header stores QImage objects directly.
#include <QImage>

#include "level.h"
#include "player.h"

class Story_Mode_Tutorial : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit Story_Mode_Tutorial(QWidget *parent = nullptr);
    void Set_Move_Left_Pressed(bool pressed);
    void Set_Move_Right_Pressed(bool pressed);
    void Set_Move_Crouch_Pressed(bool pressed);
    void Set_Move_Climb_Pressed(bool pressed);
    void Set_Move_Jump_Pressed(bool pressed);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    // This is a camera variableused to scroolll, basically follow the lil guy on the screen at all times
    float Camera_X = 0;
    float Camera_Y = 0;

    QTimer *Game_Timer;

    QTimer *Game_Loop_Timer;

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
        };

    level Current_Level;
    Player Story_Player;

private slots:
    void Update_Game();


};

#endif // STORY_MODE_TUTORIAL_H
