#include "story_mode_tutorial.h"

#include <QPainter>
#include <QtDebug>

Story_Mode_Tutorial::Story_Mode_Tutorial(QWidget *parent)
    : QOpenGLWidget(parent)
{
    Background_Image = QImage(":/Images/Background/Space_BG_7.png");
    qDebug() << "Background loaded?" << !Background_Image.isNull();

    Player_Sheet = QImage(":/Images/Characters/Walking_Gray.png");

    Game_Timer = new QTimer(this);
    connect(Game_Timer, SIGNAL(timeout()), this, SLOT(Update_Game()));
    Game_Timer->start(17);
}

void Story_Mode_Tutorial::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0.02f, 0.02f, 0.08f, 1.0f);
}

void Story_Mode_Tutorial::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void Story_Mode_Tutorial::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    QPainter painter(this);

    // background
    painter.drawImage(rect(), Background_Image);

    // OpenGL platforms
    painter.beginNativePainting();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width(), height(), 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(0.2f, 0.2f, 0.2f);

    float platformY = height() * 0.70f;
    float platformH = 35.0f;

    Draw_Rect(100, platformY, 300, platformH);
    Draw_Rect(520, platformY, 300, platformH);

    painter.endNativePainting();

    // player
    int sourceX = Player_Frame * Player_Frame_W;
    int sourceY = 0;

    QRect sourceRect(sourceX, sourceY, Player_Frame_W, Player_Frame_H);
    QRect targetRect(Player_X, Player_Y, Player_Frame_W, Player_Frame_H);

    painter.drawImage(targetRect, Player_Sheet, sourceRect);

    painter.end();
}

void Story_Mode_Tutorial::keyPressEvent(QKeyEvent *event)
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

void Story_Mode_Tutorial::keyReleaseEvent(QKeyEvent *event)
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

void Story_Mode_Tutorial::Draw_Rect(float x, float y, float w, float h)
{
    glBegin(GL_QUADS);

    glVertex2f(x,     y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x,     y + h);

    glEnd();
}

void Story_Mode_Tutorial::Update_Game()
{
    float dt = 0.017f;

    // left / right movement
    if (Move_Left_Pressed == 1)
    {
        Player_Vx = -500;
    }
    else if (Move_Right_Pressed == 1)
    {
        Player_Vx = 500;
    }
    else
    {
        Player_Vx = 0;
    }

    // jump
    if (Move_Jump_Pressed == 1 && Player_Grounded == true)
    {
        Player_Vy = -700;
        Player_Grounded = false;
    }

    // gravity
    Player_Vy += Gravity * dt;

    // update position
    Player_X += Player_Vx * dt;
    Player_Y += Player_Vy * dt;

    // platform floor
    float platformY = height() * 0.70f;

    if (Player_Y + Player_Frame_H > platformY)
    {
        Player_Y = platformY - Player_Frame_H;
        Player_Vy = 0;
        Player_Grounded = true;
    }

    // animation
    Player_Frame++;

    if (Player_Frame >= Player_Frame_Count)
    {
        Player_Frame = 0;
    }

    update();
}
