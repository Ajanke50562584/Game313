#ifndef SINGLE_PLAYER_TUTORIAL_SCENE_H
#define SINGLE_PLAYER_TUTORIAL_SCENE_H

#include <QWidget>
#include <QTimer>
#include <QKeyEvent>
#include <QResizeEvent>
#include <QPaintEvent>
#include <QShowEvent>

#include "object_physics_body.h"
#include "player.h"


class Single_Player_Tutorial_Scene : public QWidget
{
    Q_OBJECT

public:
    explicit Single_Player_Tutorial_Scene(QWidget *parent = nullptr);


protected:

    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;


private:

    QTimer *Game_Loop_Timer;

    bool Move_Left_Pressed = 0;
    bool Move_Right_Pressed = 0;
    bool Move_Crouch_Pressed = 0;
    bool Move_Climb_Pressed = 0;
    bool Move_Jump_Pressed = 0;
    bool Player_Spawned = 0;

    Player *User_Player;

private slots:
    void Update_Scene();


};

#endif // SINGLE_PLAYER_TUTORIAL_SCENE_H
