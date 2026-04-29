#ifndef STORY_MODE_TUTORIAL_H
#define STORY_MODE_TUTORIAL_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>
#include <QKeyEvent>

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

    int Player_Frame = 0;
    int Player_Frame_Count = 8;

    int Player_Frame_W = 32;
    int Player_Frame_H = 32;

    float Player_X = 150;
    float Player_Y = 350;

    QTimer *Game_Timer;

    //moving the player
    QTimer *Game_Loop_Timer;

    bool Move_Left_Pressed = 0;
    bool Move_Right_Pressed = 0;
    bool Move_Crouch_Pressed = 0;
    bool Move_Climb_Pressed = 0;
    bool Move_Jump_Pressed = 0;
    bool Player_Spawned = 0;
    float Player_Vx = 0;
    float Player_Vy = 0;

    float Gravity = 1200.0f;
    bool Player_Grounded = false;

    QImage Background_Image;

     void Draw_Rect(float x, float y, float w, float h);

private slots:
    void Update_Game();


};

#endif // STORY_MODE_TUTORIAL_H
