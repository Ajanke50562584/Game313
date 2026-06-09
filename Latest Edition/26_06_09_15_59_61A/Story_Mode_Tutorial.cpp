    #include "Story_Mode_Tutorial.h"

    #include <QPainter>
    #include <QtDebug>

    // These image paths come directly from Resources.qrc.
    static const char *TUTORIAL_BACKGROUND_PATH = ":/Images/Background/Space_BG_10.png";
    // Base width used for tutorial world coordinates before scaling to the current window.
    static const float TUTORIAL_DESIGN_WIDTH = 1280.0f;
    // Base height used for tutorial world coordinates before scaling to the current window.
    static const float TUTORIAL_DESIGN_HEIGHT = 720.0f;

    Story_Mode_Tutorial::Story_Mode_Tutorial(QWidget *parent)
        : QOpenGLWidget(parent)
    {
        Background_Image = QImage(TUTORIAL_BACKGROUND_PATH);
        qDebug() << "Background loaded?" << !Background_Image.isNull();
        Story_Player.Load_Story_Mode_Images();

        Game_Timer = new QTimer(this);
        connect(Game_Timer, SIGNAL(timeout()), this, SLOT(Update_Game())); // basically update game is where we usually have timertick
        Game_Timer->start(17);//roughly 60fps

        //get grid and platforms and player spawn point
        Current_Level.Load_Tutorial_Level();
        Story_Player.Set_Player_Spawn(Current_Level.Get_Player_Spawn());
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

        QPainter painter(this); //used to draw imaged like the background or the platforms

        // Scale tutorial size; keeps squares square even when resize which is nota bene when grid working
        float Scale = qMin((float)width() / TUTORIAL_DESIGN_WIDTH,(float)height() / TUTORIAL_DESIGN_HEIGHT);
        float View_Offset_X = (width() - TUTORIAL_DESIGN_WIDTH * Scale) / 2.0f;
        float View_Offset_Y = (height() - TUTORIAL_DESIGN_HEIGHT * Scale) / 2.0f;

        // Draw background without stretching ti; gets the scale of how large it needs to be to cover the widget and then chooses the larger scale to be bigger not smaller
        float Background_Scale = qMax((float)width() / Background_Image.width(),((float)height()) / Background_Image.height());
        //make scale even bigger to be sure in case my math fails me once more
        Background_Scale = Background_Scale * 1.25f;
        //the actual background size:
        int Background_W = Background_Image.width() * Background_Scale;
        int Background_H = Background_Image.height() * Background_Scale;

        // Move the background slower than the player because space is far dude
        int Background_Offset_X = (int)(Camera_X * 0.4f * Scale);
        int Background_Offset_Y = (int)(Camera_Y * 0.4f * Scale);
        //allows for background to loop
        Background_Offset_X = Background_Offset_X % Background_W;
        //calcs max space before background runs out vertically
        int Max_Background_Offset_Y = Background_H - height();
        // If the background is somehow smaller than the window, prevent a negative value
        if (Max_Background_Offset_Y < 0)
        {
            Max_Background_Offset_Y = 0;
        }
        // Clamp vertical movement so the background does not repeat vertically.
        Background_Offset_Y = qBound(0, Background_Offset_Y, Max_Background_Offset_Y);

        int Background_Start_X = -Background_Offset_X;
        int Background_Start_Y = -Background_Offset_Y;

        // Coppies for loopings sake
        painter.drawImage(QRect(Background_Start_X, Background_Start_Y, Background_W, Background_H), Background_Image);
        painter.drawImage(QRect(Background_Start_X + Background_W, Background_Start_Y, Background_W, Background_H), Background_Image);
        painter.drawImage(QRect(Background_Start_X - Background_W, Background_Start_Y, Background_W, Background_H), Background_Image);

        Current_Level.Draw_Platforms(painter,Camera_X,Camera_Y,Scale,View_Offset_X,View_Offset_Y);
        Story_Player.Draw_Story_Mode_Player(painter,Camera_X,Camera_Y,Scale,View_Offset_X,View_Offset_Y);

        painter.end();
    }

    void Story_Mode_Tutorial::Set_Move_Left_Pressed(bool pressed)
    {
        Story_Player.Set_Move_Left_Pressed(pressed);
    }

    void Story_Mode_Tutorial::Set_Move_Right_Pressed(bool pressed)
    {
        Story_Player.Set_Move_Right_Pressed(pressed);
    }

    void Story_Mode_Tutorial::Set_Move_Crouch_Pressed(bool pressed)
    {
        Story_Player.Set_Move_Crouch_Pressed(pressed);
    }

    void Story_Mode_Tutorial::Set_Move_Climb_Pressed(bool pressed)
    {
        Story_Player.Set_Move_Climb_Pressed(pressed);
    }

    void Story_Mode_Tutorial::Set_Move_Jump_Pressed(bool pressed)
    {
        Story_Player.Set_Move_Jump_Pressed(pressed);
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
        return qMax(32, (int)(Story_Player.Get_Player_World_Size() * (width() / TUTORIAL_DESIGN_WIDTH)));
    }

    void Story_Mode_Tutorial::Update_Game()
    {
        float dt = 0.0085f;
        // Fallback floor stays at the bottom of the level so platforms handle the actual landing.
        float platformY = Current_Level.Get_Level_Height();

        //fisika
        Story_Player.Update_Story_Mode_Player(dt);
        //fisika
        Current_Level.Handle_Player_Collision(Story_Player.Get_Player_Physics_Body());
        //fisika
        Story_Player.Handle_Story_Mode_Floor(platformY);

        //makes sure the camera is following the character
        Camera_X = Story_Player.Get_Player_World_X() - TUTORIAL_DESIGN_WIDTH * 0.35f;
        Camera_Y = Story_Player.Get_Player_World_Y() - TUTORIAL_DESIGN_HEIGHT * 0.50f;
        // clamp camera
        if (Camera_X < 0)
        {
            Camera_X = 0;
        }
        if (Camera_Y < 0)
        {
            Camera_Y = 0;
        }
        // Calculate the furthest camera is allowed to move
        float Max_Camera_X = Current_Level.Get_Level_Width() - TUTORIAL_DESIGN_WIDTH;
        float Max_Camera_Y = Current_Level.Get_Level_Height() - TUTORIAL_DESIGN_HEIGHT;

        // Stop the camera from going too far right/
        if (Camera_X > Max_Camera_X)
        {
            Camera_X = Max_Camera_X;
        }

        // Stop the camera from going too far down
        if (Camera_Y > Max_Camera_Y)
        {
            Camera_Y = Max_Camera_Y;
        }

        update(); //udate the gl
    }
