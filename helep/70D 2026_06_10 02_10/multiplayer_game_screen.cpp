#include "multiplayer_game_screen.h"
#include "leaderboard_data.h"

#include <QPainter>
#include <QtDebug>
#include <QMouseEvent>

static const char *MULTIPLAYER_BACKGROUND_PATH = ":/Images/Background/Space_BG_10.png";
static const float MULTIPLAYER_DESIGN_WIDTH = 1280.0f;
static const float MULTIPLAYER_DESIGN_HEIGHT = 720.0f;

Multiplayer_Game_Screen::Multiplayer_Game_Screen(QWidget *parent)
    : QOpenGLWidget(parent)
{
    Background_Image = QImage(MULTIPLAYER_BACKGROUND_PATH);
    qDebug() << "Multiplayer background loaded?" << !Background_Image.isNull();

    Local_Player.Load_Story_Mode_Images();

    Game_Timer = new QTimer(this);
    connect(Game_Timer, SIGNAL(timeout()), this, SLOT(Update_Game()));
    Game_Timer->start(17);

    Current_Level.Load_Multiplayer_Level();
    Local_Player.Set_Player_Spawn(Current_Level.Get_Player_Spawn());
    Course_Timer.restart();
    Course_Running = true;
    Course_Finished = false;
    Course_Lost = false;
}

void Multiplayer_Game_Screen::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.02f, 0.02f, 0.08f, 1.0f);
}

void Multiplayer_Game_Screen::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void Multiplayer_Game_Screen::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    QPainter painter(this);

    float Scale = qMin((float)width() / MULTIPLAYER_DESIGN_WIDTH,(float)height() / MULTIPLAYER_DESIGN_HEIGHT);
    float View_Offset_X = (width() - MULTIPLAYER_DESIGN_WIDTH * Scale) / 2.0f;
    float View_Offset_Y = (height() - MULTIPLAYER_DESIGN_HEIGHT * Scale) / 2.0f;

    float Background_Scale = qMax((float)width() / Background_Image.width(),((float)height()) / Background_Image.height());
    Background_Scale = Background_Scale * 1.25f;

    int Background_W = Background_Image.width() * Background_Scale;
    int Background_H = Background_Image.height() * Background_Scale;

    int Background_Offset_X = (int)(Camera_X * 0.4f * Scale);
    int Background_Offset_Y = (int)(Camera_Y * 0.4f * Scale);
    Background_Offset_X = Background_Offset_X % Background_W;

    int Max_Background_Offset_Y = Background_H - height();
    if(Max_Background_Offset_Y < 0)
        Max_Background_Offset_Y = 0;

    Background_Offset_Y = qBound(0, Background_Offset_Y, Max_Background_Offset_Y);

    int Background_Start_X = -Background_Offset_X;
    int Background_Start_Y = -Background_Offset_Y;

    painter.drawImage(QRect(Background_Start_X, Background_Start_Y, Background_W, Background_H), Background_Image);
    painter.drawImage(QRect(Background_Start_X + Background_W, Background_Start_Y, Background_W, Background_H), Background_Image);
    painter.drawImage(QRect(Background_Start_X - Background_W, Background_Start_Y, Background_W, Background_H), Background_Image);

    Current_Level.Draw_Platforms(painter, Camera_X, Camera_Y, Scale, View_Offset_X, View_Offset_Y);
    Local_Player.Draw_Story_Mode_Player(painter, Camera_X, Camera_Y, Scale, View_Offset_X, View_Offset_Y);
    Draw_Hud(painter);

    if(Leaderboard_Visible == true)
        Draw_Leaderboard_Overlay(painter);

    painter.end();
}

void Multiplayer_Game_Screen::Set_Move_Left_Pressed(bool pressed)
{
    Local_Player.Set_Move_Left_Pressed(pressed);
}

void Multiplayer_Game_Screen::Set_Move_Right_Pressed(bool pressed)
{
    Local_Player.Set_Move_Right_Pressed(pressed);
}

void Multiplayer_Game_Screen::Set_Move_Crouch_Pressed(bool pressed)
{
    Local_Player.Set_Move_Crouch_Pressed(pressed);
}

void Multiplayer_Game_Screen::Set_Move_Climb_Pressed(bool pressed)
{
    Local_Player.Set_Move_Climb_Pressed(pressed);
}

void Multiplayer_Game_Screen::Set_Move_Jump_Pressed(bool pressed)
{
    Local_Player.Set_Move_Jump_Pressed(pressed);
}

void Multiplayer_Game_Screen::Set_Interact_Pressed(bool pressed)
{
    Interact_Pressed = pressed;
}

void Multiplayer_Game_Screen::Set_Leaderboard_Visible(bool visible)
{
    Leaderboard_Visible = visible;
}

void Multiplayer_Game_Screen::Handle_Mouse_Button(int button, bool pressed)
{
    if(button == Qt::LeftButton)
    {
        Melee_Pressed = pressed;
        if(pressed)
            qDebug() << "Multiplayer melee attack";
    }
    else if(button == Qt::RightButton)
    {
        Ranged_Pressed = pressed;
        if(pressed)
            qDebug() << "Multiplayer ranged attack";
    }
}

void Multiplayer_Game_Screen::Draw_Leaderboard_Overlay(QPainter &painter)
{
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing, true);

    QRectF Panel(width() / 2 - 300, height() / 2 - 210, 600, 420);
    painter.setPen(QColor(245,245,255,210));
    painter.setBrush(QColor(15,20,35,230));
    painter.drawRoundedRect(Panel, 12, 12);

    QFont Title_Font("OCR A Extended");
    Title_Font.setPointSize(qMax(14, width() / 70));
    painter.setFont(Title_Font);
    painter.setPen(QColor(235,235,245));
    painter.drawText(Panel.adjusted(18, 14, -18, -350), Qt::AlignCenter, "Leaderboard");

    QFont Text_Font("OCR A Extended");
    Text_Font.setPointSize(qMax(9, width() / 105));
    painter.setFont(Text_Font);

    QList<Leaderboard_Entry> Entries = Leaderboard_Data::Get_Entries();
    QString Text;

    if(Entries.isEmpty())
    {
        Text = "No course results yet.";
    }
    else
    {
        for(int i = 0; i < Entries.size() && i < 8; i++)
        {
            Text += QString("%1. %2    %3    %4\n")
                    .arg(i + 1)
                    .arg(Entries[i].Player_Name)
                    .arg(Leaderboard_Data::Format_Time(Entries[i].Time_Ms))
                    .arg(Entries[i].Result);
        }
    }

    Text += "\nRelease Tab to return.";

    painter.drawText(Panel.adjusted(28, 80, -28, -28),
                     Qt::AlignLeft | Qt::AlignTop,
                     Text);

    painter.restore();
}

void Multiplayer_Game_Screen::Draw_Hud(QPainter &painter)
{
    int Time_Ms = Final_Time_Ms;

    if(Course_Running == true)
        Time_Ms = (int)Course_Timer.elapsed();

    painter.save();
    painter.setRenderHint(QPainter::Antialiasing, true);

    QFont Hud_Font("OCR A Extended");
    Hud_Font.setPointSize(qMax(10, width() / 95));
    painter.setFont(Hud_Font);

    QRectF Timer_Rect(18, 18, qMax(260, width() / 4), 42);
    painter.setPen(QColor(235,235,245));
    painter.setBrush(QColor(15,20,35,210));
    painter.drawRoundedRect(Timer_Rect, 8, 8);
    painter.drawText(Timer_Rect.adjusted(12, 0, -12, 0),
                     Qt::AlignVCenter | Qt::AlignLeft,
                     "Time: " + Leaderboard_Data::Format_Time(Time_Ms));

    if(Course_Finished == true || Course_Lost == true)
    {
        QRectF Result_Rect(width() / 2 - 220, 72, 440, 54);
        painter.setBrush(QColor(0,74,113,230));
        painter.drawRoundedRect(Result_Rect, 10, 10);
        painter.drawText(Result_Rect, Qt::AlignCenter,
                         Course_Finished == true ? "Course Complete" : "Course Failed");
    }

    painter.restore();
}

void Multiplayer_Game_Screen::Update_Game()
{
    float dt = 0.0085f;
    float platformY = Current_Level.Get_Level_Height();

    const float Standing_Width = Local_Player.Get_Player_World_Size() * 0.42f;
    const float Standing_Height = Local_Player.Get_Player_World_Size() * 0.48f;
    Object_Physics_Body &Player_Body = Local_Player.Get_Player_Physics_Body();
    const float Stand_X = Player_Body.Get_X_Pos();
    const float Stand_Y = Player_Body.Get_Bottom() - Standing_Height;
    const bool Can_Stand_Up = !Current_Level.Is_Rect_Colliding_With_Platform(Stand_X,
                                                                              Stand_Y,
                                                                              Standing_Width,
                                                                              Standing_Height);
    Local_Player.Set_Can_Stand_Up(Can_Stand_Up);

    if(Course_Running == true)
    {
        Local_Player.Update_Story_Mode_Player(dt);
        Current_Level.Handle_Player_Collision(Local_Player.Get_Player_Physics_Body());
        Check_Win_Lose_Conditions();

        if(Course_Running == true)
            Local_Player.Handle_Story_Mode_Floor(platformY);
    }

    Camera_X = Local_Player.Get_Player_World_X() - MULTIPLAYER_DESIGN_WIDTH * 0.35f;
    Camera_Y = Local_Player.Get_Player_World_Y() - MULTIPLAYER_DESIGN_HEIGHT * 0.50f;

    if(Camera_X < 0)
        Camera_X = 0;

    if(Camera_Y < 0)
        Camera_Y = 0;

    float Max_Camera_X = Current_Level.Get_Level_Width() - MULTIPLAYER_DESIGN_WIDTH;
    float Max_Camera_Y = Current_Level.Get_Level_Height() - MULTIPLAYER_DESIGN_HEIGHT;

    if(Camera_X > Max_Camera_X)
        Camera_X = Max_Camera_X;

    if(Camera_Y > Max_Camera_Y)
        Camera_Y = Max_Camera_Y;

    update();
}

void Multiplayer_Game_Screen::Check_Win_Lose_Conditions()
{
    Object_Physics_Body &Body = Local_Player.Get_Player_Physics_Body();
    QRectF Player_Rect(Body.Get_X_Pos(), Body.Get_Y_Pos(), Body.Get_Width(), Body.Get_Height());

    QRectF Finish_Rect(Current_Level.Get_Finish_Pos().x(),
                       Current_Level.Get_Finish_Pos().y(),
                       Current_Level.Get_Tile_Size(),
                       Current_Level.Get_Tile_Size());

    if(Player_Rect.intersects(Finish_Rect))
    {
        Final_Time_Ms = (int)Course_Timer.elapsed();
        Course_Running = false;
        Course_Finished = true;
        Leaderboard_Data::Add_Entry("Player", Final_Time_Ms, "Win");
        return;
    }

    QVector<QPointF> Hazards = Current_Level.Get_Hazard_Spawns();
    for(int i = 0; i < Hazards.size(); i++)
    {
        QRectF Hazard_Rect(Hazards[i].x(), Hazards[i].y(),
                           Current_Level.Get_Tile_Size(),
                           Current_Level.Get_Tile_Size());

        if(Player_Rect.intersects(Hazard_Rect))
        {
            Final_Time_Ms = (int)Course_Timer.elapsed();
            Course_Running = false;
            Course_Lost = true;
            Leaderboard_Data::Add_Entry("Player", Final_Time_Ms, "Lose");
            return;
        }
    }

    if(Body.Get_Bottom() > Current_Level.Get_Level_Height())
    {
        Final_Time_Ms = (int)Course_Timer.elapsed();
        Course_Running = false;
        Course_Lost = true;
        Leaderboard_Data::Add_Entry("Player", Final_Time_Ms, "Lose");
    }
}
