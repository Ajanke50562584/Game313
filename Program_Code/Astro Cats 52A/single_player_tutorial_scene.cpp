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

    float Old_Width,Old_Height,New_Width,New_Height;

    Old_Width = event->oldSize().width();
    Old_Height = event->oldSize().height();
    New_Width = event->size().width();
    New_Height = event->size().height();

    if(Player_Spawned==0)
    {
    float Spawn_X = width()*0.05f;
    float Spawn_Y = height()-User_Player->Get_Player_Height()-0.05f*height();

    User_Player->Get_Player_Physics_Body().Set_Pos(Spawn_X,Spawn_Y);
    Player_Spawned = 1;
    User_Player->Get_Player_Physics_Body().Set_Grounded_State(1);
    }

    else if(Player_Spawned==1)
    {
        if(Old_Height>0 && Old_Width>0)
        {
           float Current_Player_X_Pos = User_Player->Get_Player_Physics_Body().Get_X_Pos();
           float Current_Player_Y_Pos = User_Player->Get_Player_Physics_Body().Get_Y_Pos();

           float Scaled_Player_X_Pos = Current_Player_X_Pos*(New_Width/Old_Width);
           float Scaled_Player_Y_Pos = Current_Player_Y_Pos*(New_Height/Old_Height);

           User_Player->Get_Player_Physics_Body().Set_Pos(Scaled_Player_X_Pos,Scaled_Player_Y_Pos);
        }
    }

    QWidget::resizeEvent(event);
}

void Single_Player_Tutorial_Scene::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    setFocus(Qt::OtherFocusReason);
}

void Single_Player_Tutorial_Scene::keyPressEvent(QKeyEvent *event)
{

    switch(event->key())
    {
    case Qt::Key_W:
    case Qt::Key_Up:
        qDebug() << "climb";
        Move_Climb_Pressed = 1;
        break;

    case Qt::Key_A:
    case Qt::Key_Left:
        qDebug() << "Move Left";
        Move_Left_Pressed = 1;
        break;

    case Qt::Key_S:
    case Qt::Key_Down:
        qDebug() << "croutch";
        Move_Crouch_Pressed = 1;
        break;

    case Qt::Key_D:
    case Qt::Key_Right:
        qDebug() << "Move Right";
        Move_Right_Pressed = 1;
        break;

    case Qt::Key_Space:
        qDebug() << "Jump";
        Move_Jump_Pressed = 1;
        break;
    }
        QWidget::keyPressEvent(event);
}

void Single_Player_Tutorial_Scene::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_W:
    case Qt::Key_Up:
        qDebug() << "climb Released";
        Move_Climb_Pressed = 0;
        break;

    case Qt::Key_A:
    case Qt::Key_Left:
        qDebug() << "Move Left Released";
        Move_Left_Pressed = 0;
        break;

    case Qt::Key_S:
    case Qt::Key_Down:
        qDebug() << "croutch Released";
        Move_Crouch_Pressed = 0;
        break;

    case Qt::Key_D:
    case Qt::Key_Right:
        qDebug() << "Move Right Released";
        Move_Right_Pressed = 0;
        break;

    case Qt::Key_Space:
        qDebug() << "Jump Released";
        Move_Jump_Pressed = 0;
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

     if(Move_Jump_Pressed == 1)
     {
        User_Player->Get_Player_Physics_Body().Jump(300);
        User_Player->Get_Player_Physics_Body().Set_Grounded_State(0);
     }

     float Floor_Y = height() - User_Player->Get_Player_Height() - 0.05f * height();


        User_Player->Get_Player_Physics_Body().Apply_Gravity(User_Player->Get_Player_Physics_Body().Base_Gravity,0.017f);




    User_Player->Get_Player_Physics_Body().Update_Pos(0.017f); // matches timer

        if(User_Player->Get_Player_Physics_Body().Get_Y_Pos() > Floor_Y)
        {
             User_Player->Get_Player_Physics_Body().Set_Grounded_State(1);
             User_Player->Get_Player_Physics_Body().Set_Pos(User_Player->Get_Player_Physics_Body().Get_X_Pos(), Floor_Y);
             User_Player->Get_Player_Physics_Body().Set_Vy(0);

        }

    update();
}
