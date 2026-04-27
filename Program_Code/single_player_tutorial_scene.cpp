#include "single_player_tutorial_scene.h"
#include <QPainter>
#include <QRect>
#include <QDebug>


Single_Player_Tutorial_Scene::Single_Player_Tutorial_Scene(QWidget *parent)
    : QWidget(parent)
{

    User_Player = new Player(this);

    Game_Loop_Timer = new QTimer(this);
    connect(Game_Loop_Timer,SIGNAL(timeout()),this,SLOT(Update_Scene()));
    Game_Loop_Timer->start(17); //roughly 60 fps

    setFocusPolicy(Qt::StrongFocus);

    User_Player->Get_Player_Physics_Body().Set_Pos(width()*0.1f,height()*0.1f);

}

void Single_Player_Tutorial_Scene::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter Tut_Story_Scene_Painter(this);
    Tut_Story_Scene_Painter.drawRect(User_Player->Get_Player_Physics_Body().Get_X_Pos(),
                                     User_Player->Get_Player_Physics_Body().Get_Y_Pos(),
                                     User_Player->Get_Player_Width(),
                                     User_Player->Get_Player_Height());
}

void Single_Player_Tutorial_Scene::resizeEvent(QResizeEvent *event)
{
    User_Player->Set_Player_Width(width()*0.15f);
    User_Player->Set_Player_Height(height()*0.15f);

    if(User_Player->Get_Player_Physics_Body().Get_X_Pos()==0 && User_Player->Get_Player_Physics_Body().Get_Y_Pos()== 0)
    User_Player->Get_Player_Physics_Body().Set_Pos(width()*0.1f,height()*0.1f);

    QWidget::resizeEvent(event);
}

void Single_Player_Tutorial_Scene::keyPressEvent(QKeyEvent *event)
{

    switch(event->key())
    {
    case Qt::Key_W:
        qDebug() << "climb";
        Move_Climb_Pressed = 1;
        break;

    case Qt::Key_A:
        qDebug() << "Move Left";
        Move_Left_Pressed = 1;
        break;

    case Qt::Key_S:
        qDebug() << "croutch";
        Move_Crouch_Pressed = 1;
        break;

    case Qt::Key_D:
        qDebug() << "Move Right";
        Move_Right_Pressed = 1;
        break;
    }
        QWidget::keyPressEvent(event);
}

void Single_Player_Tutorial_Scene::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_W:
        qDebug() << "climb";
        Move_Climb_Pressed = 0;
        break;

    case Qt::Key_A:
        qDebug() << "Move Left";
        Move_Left_Pressed = 0;
        break;

    case Qt::Key_S:
        qDebug() << "croutch";
        Move_Crouch_Pressed = 0;
        break;

    case Qt::Key_D:
        qDebug() << "Move Right";
        Move_Right_Pressed = 0;
        break;
    }
     QWidget::keyReleaseEvent(event);
}
void Single_Player_Tutorial_Scene::Update_Scene()
{
    if(Move_Left_Pressed == 1)
        User_Player->Get_Player_Physics_Body().Set_Vx(-500);

    else if(Move_Right_Pressed == 1)
        User_Player->Get_Player_Physics_Body().Set_Vx(500);

    else if(Move_Left_Pressed == 0 && Move_Right_Pressed == 0 )
        User_Player->Get_Player_Physics_Body().Set_Vx(0);

    User_Player->Get_Player_Physics_Body().Update_Pos(0.017f); // matches timer

    update();
}
