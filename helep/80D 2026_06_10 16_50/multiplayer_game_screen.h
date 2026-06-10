#ifndef MULTIPLAYER_GAME_SCREEN_H
#define MULTIPLAYER_GAME_SCREEN_H

#include <QOpenGLWidget> // game screen widget
#include <QOpenGLFunctions> // lets us call gl stuff
#include <QTimer> // game loop timer
#include <QImage> // background image
#include <QElapsedTimer> // race timer

#include "level.h" // multiplayer course
#include "player.h" // local player

class QPainter;

class Multiplayer_Game_Screen : public QOpenGLWidget, protected QOpenGLFunctions // multiplayer race screen
{
    Q_OBJECT // Signals and slots

public:
    explicit Multiplayer_Game_Screen(QWidget *parent = nullptr); // create screen and load multiplayer level
    void Start_Multiplayer_Level(const QString &difficulty); // load a course for chosen difficulty
    void Set_Move_Left_Pressed(bool pressed); // input from main window
    void Set_Move_Right_Pressed(bool pressed); // input from main window
    void Set_Move_Crouch_Pressed(bool pressed); // crouch input
    void Set_Move_Climb_Pressed(bool pressed); // climb input
    void Set_Move_Jump_Pressed(bool pressed); // jump input
    void Set_Interact_Pressed(bool pressed); // E key input
    void Set_Leaderboard_Visible(bool visible); // tab overlay
    void Handle_Mouse_Button(int button, bool pressed); // mouse attacks

protected:
    void initializeGL() override; // setup opengl clear colour
    void resizeGL(int w, int h) override; // keep viewport current
    void paintGL() override; // draw one frame

private:
    float Camera_X = 0; // follows local player horizontally
    float Camera_Y = 0; // follows local player vertically

    QTimer *Game_Timer; // main update timer
    QImage Background_Image; // space background
    bool Leaderboard_Visible = false; // show scores while tab held
    bool Interact_Pressed = false; // E key flag
    bool Melee_Pressed = false; // left mouse flag
    bool Ranged_Pressed = false; // right mouse flag
    QElapsedTimer Course_Timer; // stopwatch for race
    int Final_Time_Ms = 0; // saved finish/loss time
    int Time_Penalty_Ms = 0;
    int Enemy_Hit_Cooldown_Ticks = 0;
    bool Course_Running = false; // timer/game active
    bool Course_Finished = false; // reached finish
    bool Course_Lost = false; // hit hazard/fell

    level Current_Level; // race course
    Player Local_Player; // player on this computer

    void Draw_Leaderboard_Overlay(QPainter &painter); // tab score overlay
    void Draw_Round_Result_Overlay(QPainter &painter);
    void Draw_Hud(QPainter &painter); // timer and result
    void Check_Win_Lose_Conditions(); // finish/hazard/fall checks
    void Reset_Player_To_Chunk_Start();
    void Apply_Enemy_Time_Penalty();

private slots:
    void Update_Game(); // main timer tick
};

#endif // MULTIPLAYER_GAME_SCREEN_H
