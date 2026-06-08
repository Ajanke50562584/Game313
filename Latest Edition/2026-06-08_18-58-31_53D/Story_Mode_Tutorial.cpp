#include "Story_Mode_Tutorial.h"

#include <QPainter>
#include <QtDebug>

// These image paths come directly from Resources.qrc.
static const char *TUTORIAL_BACKGROUND_PATH = ":/Images/Background/Space_BG_7.png";
static const char *TUTORIAL_PLAYER_PATH = ":/Images/Characters/Walking_Gray.png";

Story_Mode_Tutorial::Story_Mode_Tutorial(QWidget *parent)
    : QOpenGLWidget(parent)
{
    Background_Image = QImage(TUTORIAL_BACKGROUND_PATH);
    qDebug() << "Background loaded?" << !Background_Image.isNull();

    Player_Sheet = QImage(TUTORIAL_PLAYER_PATH);

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

    // Draw the background to fill the current widget size.
    painter.drawImage(rect(), Background_Image);

    // OpenGL platforms use the current widget dimensions instead of fixed pixels.
    painter.beginNativePainting();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width(), height(), 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(0.2f, 0.2f, 0.2f);

    float platformY = height() * 0.70f;
    float platformH = qMax(20.0f, height() * 0.05f);
    float platformW = width() * 0.24f;
    float leftPlatformX = width() * 0.08f;
    float rightPlatformX = width() * 0.48f;

    Draw_Rect(leftPlatformX, platformY, platformW, platformH);
    Draw_Rect(rightPlatformX, platformY, platformW, platformH);

    painter.endNativePainting();

    // Source rectangle stays in sprite-sheet pixels; target rectangle scales on screen.
    int sourceX = Player_Frame * Player_Frame_W;
    int sourceY = 0;
    int playerDrawSize = Get_Player_Draw_Size();

    QRect sourceRect(sourceX, sourceY, Player_Frame_W, Player_Frame_H);
    QRect targetRect(Player_X, Player_Y, playerDrawSize, playerDrawSize);

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

int Story_Mode_Tutorial::Get_Player_Draw_Size() const
{
    return qMax(32, (96 * qMax(width(), 320)) / 1280);
}

void Story_Mode_Tutorial::Update_Game()
{
    float dt = 0.017f;
    int playerDrawSize = Get_Player_Draw_Size();

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

    // Platform floor follows the current widget height.
    float platformY = height() * 0.70f;

    if (Player_Y + playerDrawSize > platformY)
    {
        Player_Y = platformY - playerDrawSize;
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
