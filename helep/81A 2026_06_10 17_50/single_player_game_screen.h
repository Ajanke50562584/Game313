#ifndef SINGLE_PLAYER_GAME_SCREEN_H
#define SINGLE_PLAYER_GAME_SCREEN_H

#include <QOpenGLWidget> // game screen widget
#include <QOpenGLFunctions> // lets us call gl stuff
#include <QTimer> // game loop timer
#include <QKeyEvent> // input types
#include <QImage> // background image stored here
#include <QElapsedTimer> // course timer
#include <QVector> // tutorial enemy list

#include "level.h" // current course
#include "player.h" // story player
#include "enemy.h" // tutorial enemies

class QPainter;

class Single_Player_Game_Screen : public QOpenGLWidget, protected QOpenGLFunctions // actual single player run screen
{
    Q_OBJECT // Signals and slots

public:
    explicit Single_Player_Game_Screen(QWidget *parent = nullptr); // create screen and start timer
    void Start_Single_Player_Level(bool tutorialSelected); // restart/load chosen single player level
    void Start_Story_Level_Number(int levelNumber);
    void Set_Move_Left_Pressed(bool pressed); // movement input from window
    void Set_Move_Right_Pressed(bool pressed); // movement input from window
    void Set_Move_Crouch_Pressed(bool pressed); // crouch input
    void Set_Move_Climb_Pressed(bool pressed); // climb input
    void Set_Move_Jump_Pressed(bool pressed); // jump input
    void Set_Run_Pressed(bool pressed); // shift run input
    void Set_Interact_Pressed(bool pressed); // interact input
    void Set_Leaderboard_Visible(bool visible); // tab overlay
    void Handle_Mouse_Button(int button, bool pressed); // mouse attacks

protected:
    void initializeGL() override; // setup opengl background clear
    void resizeGL(int w, int h) override; // keep viewport correct
    void paintGL() override; // draw one frame

private:
    // This is a camera variableused to scroolll, basically follow the lil guy on the screen at all times
    float Camera_X = 0;
    float Camera_Y = 0;

    QTimer *Game_Timer; // main update timer

    QTimer *Game_Loop_Timer; // older game loop timer placeholder

    QImage Background_Image; // space background
    QElapsedTimer Course_Timer; // stopwatch for speedrun
    int Final_Time_Ms = 0; // saved time after finish/loss
    int Total_Run_Time_Ms = 0;
    int Current_Story_Level = 1;
    int Player_Lives = 9;
    int Damage_Cooldown_Ticks = 0;
    bool Course_Running = false; // true while timer is active
    bool Course_Finished = false; // won the course
    bool Course_Lost = false; // hit hazard/fell
    bool Tutorial_Mode = true; // shows tutorial hints
    bool Leaderboard_Visible = false; // tab overlay flag
    bool Interact_Pressed = false; // E key
    bool Melee_Pressed = false; // left mouse
    bool Ranged_Pressed = false; // right mouse

    int Get_Player_Draw_Size() const; // old helper for player size
    void Draw_Rect(float x, float y, float w, float h); // quick opengl rectangle
    void Draw_Hud(QPainter &painter); // timer and result text
    void Draw_Tutorial_Dialogue(QPainter &painter); // tutorial popup text
    void Draw_Leaderboard_Overlay(QPainter &painter); // scores while holding tab
    void Draw_Round_Result_Overlay(QPainter &painter);
    void Check_Win_Lose_Conditions(); // finish, hazard, fall checks
    void Advance_To_Next_Story_Level();
    void Damage_Player_And_Respawn();
    void Load_Tutorial_Enemies();

    //this struct is used to build the platforms
    struct Platform_Tile
        {
            float X;
            float Y;
            float W;
            float H;
            int Tile_Type; // platform, left edge, middle, right edge
        };

    level Current_Level; // loaded single player course
    Player Story_Player; // the local cat
    QVector<enemy> Tutorial_Enemies; // enemies only wired for tutorial testing

private slots:
    void Update_Game(); // main tick, called by timer


};

#endif // SINGLE_PLAYER_GAME_SCREEN_H
