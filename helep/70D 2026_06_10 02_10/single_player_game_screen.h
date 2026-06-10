#ifndef SINGLE_PLAYER_GAME_SCREEN_H
#define SINGLE_PLAYER_GAME_SCREEN_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>
#include <QKeyEvent>
// Needed because this header stores QImage objects directly.
#include <QImage>
#include <QElapsedTimer>

#include "level.h"
#include "player.h"

class QPainter;

class Single_Player_Game_Screen : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit Single_Player_Game_Screen(QWidget *parent = nullptr);
    void Start_Single_Player_Level(bool tutorialSelected);
    void Set_Move_Left_Pressed(bool pressed);
    void Set_Move_Right_Pressed(bool pressed);
    void Set_Move_Crouch_Pressed(bool pressed);
    void Set_Move_Climb_Pressed(bool pressed);
    void Set_Move_Jump_Pressed(bool pressed);
    void Set_Interact_Pressed(bool pressed);
    void Set_Leaderboard_Visible(bool visible);
    void Handle_Mouse_Button(int button, bool pressed);

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
    QElapsedTimer Course_Timer;
    int Final_Time_Ms = 0;
    bool Course_Running = false;
    bool Course_Finished = false;
    bool Course_Lost = false;
    bool Tutorial_Mode = true;
    bool Leaderboard_Visible = false;
    bool Interact_Pressed = false;
    bool Melee_Pressed = false;
    bool Ranged_Pressed = false;

    int Get_Player_Draw_Size() const;
    void Draw_Rect(float x, float y, float w, float h);
    void Draw_Hud(QPainter &painter);
    void Draw_Tutorial_Dialogue(QPainter &painter);
    void Draw_Leaderboard_Overlay(QPainter &painter);
    void Check_Win_Lose_Conditions();

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

#endif // SINGLE_PLAYER_GAME_SCREEN_H
