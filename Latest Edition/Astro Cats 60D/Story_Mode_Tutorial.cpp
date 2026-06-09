#include "Story_Mode_Tutorial.h"
#include "object_physics_body.h"
#include <QPainter>
#include <QtDebug>


static const char *TUTORIAL_BACKGROUND_PATH = ":/Images/Background/Space_BG_7.png";
static const char *TUTORIAL_PLAYER_PATH = ":/Images/Characters/Walking_Gray.png";

Story_Mode_Tutorial::Story_Mode_Tutorial(QWidget *parent)
    : QOpenGLWidget(parent)
{
    Background_Image = QImage(TUTORIAL_BACKGROUND_PATH);
    qDebug() << "Background loaded?" << !Background_Image.isNull();

    Player_Sheet = QImage(TUTORIAL_PLAYER_PATH);
    Player_Body.Set_Pos(150, 350);
    Player_Body.Base_Gravity = 1200.0f;
    Player_Body.Set_Grav_Scale(1.0f);
    Player_Body.Set_Max_Fall(1400.0f);

    Game_Timer = new QTimer(this);
    connect(Game_Timer, SIGNAL(timeout()), this, SLOT(Update_Game())); // basically update game is where we usually have timertick
    Game_Timer->start(17);

    //Static platforms for tutorial screen
    Platform_Tiles.append({100, 500, 300, 35, 0});
    Platform_Tiles.append({550, 420, 300, 35, 0});
    Platform_Tiles.append({1000, 500, 300, 35, 0});

    for(int i = 0; i < Platform_Tiles.size(); i++)
    {
        Platform_Tiles[i].Platform_Body.Set_Pos(Platform_Tiles[i].X, Platform_Tiles[i].Y);

        Platform_Tiles[i].Platform_Body.Set_Size(Platform_Tiles[i].W,Platform_Tiles[i].H);
    }
}

void Story_Mode_Tutorial::initializeGL()
{
    initializeOpenGLFunctions(); // allows us to gl sh*t out of this game

    glClearColor(0.02f, 0.02f, 0.08f, 1.0f);
}

void Story_Mode_Tutorial::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h); //makes us see the resizewd gl?????????????????
}

void Story_Mode_Tutorial::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    QPainter painter(this);


    //makes the background scroll (the 0.4f makes it slower than the rest of the elemts since its "further away", cool physics for awesomeness points wink wink
    int Background_Offset = ((int)(Camera_X * 0.4f)) % width(); //%width keeps number in the screen width
    //draw the background image at the wanted offset
    painter.drawImage(QRect(-Background_Offset, 0, width(), height()), Background_Image);
    painter.drawImage(QRect(width() - Background_Offset, 0, width(), height()), Background_Image);

    //nonrepeating version

//    float Scale = (float)height() / Background_Image.height();
//    int Scaled_Background_Width = Background_Image.width() * Scale;

//    int Background_X = -Camera_X;

//    painter.drawImage(QRect(Background_X, 0,
//                            Scaled_Background_Width,
//                            height()),
//                      Background_Image);

    // OpenGL platforms use the current widget dimensions instead of fixed pixels.
    painter.beginNativePainting();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width(), height(), 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(0.2f, 0.2f, 0.2f);

    //draws amount of platforms in the list where they need to be initially
    for(int i = 0; i < Platform_Tiles.size(); i++)
    {
        Platform_Tile Tile = Platform_Tiles[i];

        Draw_Rect(Tile.X - Camera_X,
                  Tile.Y - Camera_Y,
                  Tile.W,
                  Tile.H);
    }

    painter.endNativePainting();

    // Source rectangle stays in sprite-sheet pixels; target rectangle scales on screen.
    int sourceX = Player_Frame * Player_Frame_W;
    int sourceY = 0;
    int playerDrawSize = Get_Player_Draw_Size();

    QRect sourceRect(sourceX, sourceY, Player_Frame_W, Player_Frame_H);
    //draw player in center of camera
    QRect targetRect(Player_Body.Get_X_Pos() - Camera_X - (playerDrawSize * 0.20f),
                     Player_Body.Get_Y_Pos() - Camera_Y - (playerDrawSize * 0.15f),
                     playerDrawSize,
                     playerDrawSize);

    //gl again only draw
    painter.drawImage(targetRect, Player_Sheet, sourceRect);

    painter.end();
}

void Story_Mode_Tutorial::keyPressEvent(QKeyEvent *event)
{
    // basic movement based on wasd and arrows with space as jump
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
    // basially using gl to "drw" the window as a rectangle and resizing it based on the coordinates of the corners
    glBegin(GL_QUADS);

    glVertex2f(x,     y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x,     y + h);

    glEnd();
}

int Story_Mode_Tutorial::Get_Player_Draw_Size() const
{
    //get the proportional size of the character in verhouding met die window size
    return qMax(32, (96 * qMax(width(), 320)) / 1280);
}

void Story_Mode_Tutorial::Update_Game()
{
    float dt = 0.017f;
    int playerDrawSize = Get_Player_Draw_Size();
    Player_Body.Set_Size(playerDrawSize * 0.55f,playerDrawSize * 0.85f);

    if(Move_Left_Pressed == 1)
    {
        Player_Body.Set_Vx(-500);
    }
    else if(Move_Right_Pressed == 1)
    {
        Player_Body.Set_Vx(500);
    }
    else
    {
        Player_Body.Set_Vx(0);
    }

    if(Move_Jump_Pressed == 1)
    {
        Player_Body.Jump(700);
    }

    Player_Body.Set_Grounded_State(false);

    Player_Body.Update(dt);

    //makes sure the camera is following the character
    Camera_X = Player_Body.Get_X_Pos() - width() * 0.35f;

    //makes sure camera doesnt move off before the level started
    if(Camera_X < 0)
        Camera_X = 0;

    //stops the camera before it moves off the background image
//    float Max_Camera_X = Scaled_Background_Width - width();

//    if(Camera_X > Max_Camera_X)
//        Camera_X = Max_Camera_X;

    for(int i = 0; i < Platform_Tiles.size(); i++)
    {
        Platform_Tile Tile = Platform_Tiles[i];

        Player_Body.Handle_Collision(Tile.Platform_Body.Get_X_Pos(),
                                     Tile.Platform_Body.Get_Y_Pos(),
                                     Tile.Platform_Body.Get_Width(),
                                     Tile.Platform_Body.Get_Height());
    }

    // animation
    Player_Frame++;

    if (Player_Frame >= Player_Frame_Count)
    {
        Player_Frame = 0;
    }
    update(); //udate the gl???????????????????????????????????
}
