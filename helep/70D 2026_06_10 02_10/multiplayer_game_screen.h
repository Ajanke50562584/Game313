#ifndef MULTIPLAYER_GAME_SCREEN_H
#define MULTIPLAYER_GAME_SCREEN_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>
#include <QImage>
#include <QElapsedTimer>

#include "level.h"
#include "player.h"

class QPainter;

class Multiplayer_Game_Screen : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit Multiplayer_Game_Screen(QWidget *parent = nullptr);
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
    float Camera_X = 0;
    float Camera_Y = 0;

    QTimer *Game_Timer;
    QImage Background_Image;
    bool Leaderboard_Visible = false;
    bool Interact_Pressed = false;
    bool Melee_Pressed = false;
    bool Ranged_Pressed = false;
    QElapsedTimer Course_Timer;
    int Final_Time_Ms = 0;
    bool Course_Running = false;
    bool Course_Finished = false;
    bool Course_Lost = false;

    level Current_Level;
    Player Local_Player;

    void Draw_Leaderboard_Overlay(QPainter &painter);
    void Draw_Hud(QPainter &painter);
    void Check_Win_Lose_Conditions();

private slots:
    void Update_Game();
};

#endif // MULTIPLAYER_GAME_SCREEN_H
