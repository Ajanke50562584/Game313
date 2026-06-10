    #include "single_player_game_screen.h"
    #include "leaderboard_data.h"

    #include <QPainter>
    #include <QtDebug>
    #include <QMouseEvent>

    // These image paths come directly from Resources.qrc.
    static const char *TUTORIAL_BACKGROUND_PATH = ":/Images/Background/Space_BG_10.png";
    // Base width used for tutorial world coordinates before scaling to the current window.
    static const float TUTORIAL_DESIGN_WIDTH = 1280.0f;
    // Base height used for tutorial world coordinates before scaling to the current window.
    static const float TUTORIAL_DESIGN_HEIGHT = 720.0f;

    Single_Player_Game_Screen::Single_Player_Game_Screen(QWidget *parent)
        : QOpenGLWidget(parent)
    {
        Background_Image = QImage(TUTORIAL_BACKGROUND_PATH);
        qDebug() << "Background loaded?" << !Background_Image.isNull();
        Story_Player.Load_Story_Mode_Images();

        Game_Timer = new QTimer(this);
        connect(Game_Timer, SIGNAL(timeout()), this, SLOT(Update_Game())); // basically update game is where we usually have timertick
        Game_Timer->start(17);//roughly 60fps

        // Keep normal story play pointed at level 1 unless the tutorial button asks for tutorial content.
        Start_Story_Level_Number(1);
    }

    void Single_Player_Game_Screen::Start_Single_Player_Level(bool tutorialSelected)
    {
        Tutorial_Mode = tutorialSelected;

        if(tutorialSelected == true)
        {
            Current_Level.Load_Tutorial_Level();
            Current_Story_Level = 1;
            Total_Run_Time_Ms = 0;
            Player_Lives = 9;
            Load_Tutorial_Enemies();
        }
        else
        {
            Current_Story_Level = 1;
            Total_Run_Time_Ms = 0;
            Player_Lives = 9;
            Current_Level.Load_Single_Player_Level_Number(Current_Story_Level);
            Tutorial_Enemies.clear();
        }

        Camera_X = 0;
        Camera_Y = 0;
        Story_Player.Set_Player_Spawn(Current_Level.Get_Player_Spawn());
        Course_Timer.restart();
        Final_Time_Ms = 0;
        Course_Running = true;
        Course_Finished = false;
        Course_Lost = false;
        Leaderboard_Visible = false;
        Damage_Cooldown_Ticks = 0;
    }

    void Single_Player_Game_Screen::Start_Story_Level_Number(int levelNumber)
    {
        Tutorial_Mode = false;
        Current_Story_Level = qBound(1, levelNumber, 10);
        Total_Run_Time_Ms = 0;
        Player_Lives = 9;
        Current_Level.Load_Single_Player_Level_Number(Current_Story_Level);
        Tutorial_Enemies.clear();
        Camera_X = 0;
        Camera_Y = 0;
        Story_Player.Set_Player_Spawn(Current_Level.Get_Player_Spawn());
        Course_Timer.restart();
        Final_Time_Ms = 0;
        Course_Running = true;
        Course_Finished = false;
        Course_Lost = false;
        Leaderboard_Visible = false;
        Damage_Cooldown_Ticks = 0;
    }

    void Single_Player_Game_Screen::initializeGL()
    {
        initializeOpenGLFunctions(); // allows us to gl sh*t out of this game

        glClearColor(0.02f, 0.02f, 0.08f, 1.0f);
    }

    void Single_Player_Game_Screen::resizeGL(int w, int h)
    {
        glViewport(0, 0, w, h); //makes us see the resizewd gl?????????????????
    }

    void Single_Player_Game_Screen::paintGL()
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

        for(int i = 0; i < Tutorial_Enemies.size(); i++)
        {
            Tutorial_Enemies[i].Draw(painter, Camera_X, Camera_Y, Scale, View_Offset_X, View_Offset_Y);
        }

        Story_Player.Draw_Story_Mode_Player(painter,Camera_X,Camera_Y,Scale,View_Offset_X,View_Offset_Y);
        Draw_Hud(painter);

        if(Tutorial_Mode == true)
            Draw_Tutorial_Dialogue(painter);

        if(Leaderboard_Visible == true)
            Draw_Leaderboard_Overlay(painter);

        if(Course_Finished == true || Course_Lost == true)
            Draw_Round_Result_Overlay(painter);

        painter.end();
    }

    void Single_Player_Game_Screen::Set_Move_Left_Pressed(bool pressed)
    {
        Story_Player.Set_Move_Left_Pressed(pressed);
    }

    void Single_Player_Game_Screen::Set_Move_Right_Pressed(bool pressed)
    {
        Story_Player.Set_Move_Right_Pressed(pressed);
    }

    void Single_Player_Game_Screen::Set_Move_Crouch_Pressed(bool pressed)
    {
        Story_Player.Set_Move_Crouch_Pressed(pressed);
    }

    void Single_Player_Game_Screen::Set_Move_Climb_Pressed(bool pressed)
    {
        Story_Player.Set_Move_Climb_Pressed(pressed);
    }

    void Single_Player_Game_Screen::Set_Move_Jump_Pressed(bool pressed)
    {
        Story_Player.Set_Move_Jump_Pressed(pressed);
    }

    void Single_Player_Game_Screen::Set_Run_Pressed(bool pressed)
    {
        Story_Player.Set_Run_Pressed(pressed);
    }

    void Single_Player_Game_Screen::Set_Interact_Pressed(bool pressed)
    {
        Interact_Pressed = pressed;
    }

    void Single_Player_Game_Screen::Set_Leaderboard_Visible(bool visible)
    {
        Leaderboard_Visible = visible;
    }

    void Single_Player_Game_Screen::Handle_Mouse_Button(int button, bool pressed)
    {
        if(button == Qt::LeftButton)
        {
            Melee_Pressed = pressed;
            if(pressed)
                qDebug() << "Melee attack";
        }
        else if(button == Qt::RightButton)
        {
            Ranged_Pressed = pressed;
            if(pressed)
                qDebug() << "Ranged attack";
        }
    }

    void Single_Player_Game_Screen::Draw_Rect(float x, float y, float w, float h)
    {
        // basially using gl to "drw" the window as a rectangle and resizing it based on the coordinates of the corners
        glBegin(GL_QUADS);

        glVertex2f(x,     y);
        glVertex2f(x + w, y);
        glVertex2f(x + w, y + h);
        glVertex2f(x,     y + h);

        glEnd();
    }

    int Single_Player_Game_Screen::Get_Player_Draw_Size() const
    {
        //get the proportional size of the character in verhouding met die window size
        return qMax(32, (int)(Story_Player.Get_Player_World_Size() * (width() / TUTORIAL_DESIGN_WIDTH)));
    }

    void Single_Player_Game_Screen::Draw_Hud(QPainter &painter)
    {
        int Time_Ms = Final_Time_Ms;
        int Display_Total_Ms = Total_Run_Time_Ms;

        if(Course_Running == true)
        {
            Time_Ms = (int)Course_Timer.elapsed();
            Display_Total_Ms = Total_Run_Time_Ms + Time_Ms;
        }

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
                         "Level: " + QString::number(Current_Story_Level) +
                         "   Time: " + Leaderboard_Data::Format_Time(Time_Ms) +
                         "   Total: " + Leaderboard_Data::Format_Time(Display_Total_Ms) +
                         "   Lives: " + QString::number(Player_Lives));

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

    void Single_Player_Game_Screen::Draw_Tutorial_Dialogue(QPainter &painter)
    {
        painter.save();
        painter.setRenderHint(QPainter::Antialiasing, true);

        QFont Dialogue_Font("OCR A Extended");
        Dialogue_Font.setPointSize(qMax(8, width() / 125));
        painter.setFont(Dialogue_Font);

        const float Panel_W = qMin(330, width() - 36);
        const float Panel_H = qMin(126, height() - 36);
        QRectF Panel(width() - Panel_W - 18, height() - Panel_H - 18, Panel_W, Panel_H);
        painter.setPen(QColor(245,245,255,200));
        painter.setBrush(QColor(15,20,35,215));
        painter.drawRoundedRect(Panel, 10, 10);

        const float Player_X = Story_Player.Get_Player_World_X();

        QString Text;

        if(Player_X < 8 * Current_Level.Get_Tile_Size())
        {
            Text = "Boot sequence: tiny paws online.\n"
                   "A/D or Left/Right: move\n"
                   "You have 9 lives. Spend them with some dignity.";
        }
        else if(Player_X < 16 * Current_Level.Get_Tile_Size())
        {
            Text = "Low tunnel ahead. Pride must crouch first.\n"
                   "S/Down: crouch\n"
                   "Stay low until your ears clear the ceiling.";
        }
        else if(Player_X < 27 * Current_Level.Get_Tile_Size())
        {
            Text = "Gap detected. Gravity is not your friend.\n"
                   "Space: jump\n"
                   "Hold a direction while jumping to cross cleanly.";
        }
        else if(Player_X < 36 * Current_Level.Get_Tile_Size())
        {
            Text = "Climbable route ahead.\n"
                   "W/Up: climb\n"
                   "Use it when the course gets vertical.";
        }
        else if(Player_X < 45 * Current_Level.Get_Tile_Size())
        {
            Text = "Strange object nearby. Probably safe. Probably.\n"
                   "E: interact\n"
                   "Blue pickups help. Purple debuffs ruin friendships.";
        }
        else if(Player_X < 53 * Current_Level.Get_Tile_Size())
        {
            Text = "Enemy contact drill.\n"
                   "Left click: melee\n"
                   "Right click: ranged\n"
                   "Fast clears make fast times.";
        }
        else
        {
            Text = "Final stretch. Try to look heroic.\n"
                   "Hold Tab: leaderboard\n"
                   "Reach the green marker to lock in your time.";
        }

        painter.setPen(QColor(235,235,245));
        painter.drawText(Panel.adjusted(10, 8, -10, -8),
                         Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap,
                         Text);

        painter.restore();
    }

    void Single_Player_Game_Screen::Draw_Leaderboard_Overlay(QPainter &painter)
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

    void Single_Player_Game_Screen::Draw_Round_Result_Overlay(QPainter &painter)
    {
        painter.save();
        painter.setRenderHint(QPainter::Antialiasing, true);

        QRectF Panel(width() / 2 - 330, height() / 2 - 185, 660, 370);
        painter.setPen(QColor(245,245,255,220));
        painter.setBrush(QColor(15,20,35,235));
        painter.drawRoundedRect(Panel, 12, 12);

        QFont Title_Font("OCR A Extended");
        Title_Font.setPointSize(qMax(13, width() / 72));
        painter.setFont(Title_Font);
        painter.setPen(QColor(235,235,245));

        QString Title = Course_Finished ? "Round Complete" : "Run Ended";
        painter.drawText(Panel.adjusted(18, 16, -18, -310), Qt::AlignCenter, Title);

        QFont Text_Font("OCR A Extended");
        Text_Font.setPointSize(qMax(9, width() / 112));
        painter.setFont(Text_Font);

        QString Text;
        Text += "Level: " + QString::number(Current_Story_Level) + " / 10\n";
        Text += "Round time: " + Leaderboard_Data::Format_Time(Final_Time_Ms) + "\n";
        Text += "Total run time: " + Leaderboard_Data::Format_Time(Total_Run_Time_Ms) + "\n";
        Text += "Lives: " + QString::number(Player_Lives) + "\n\n";

        QList<Leaderboard_Entry> Entries = Leaderboard_Data::Get_Entries();
        for(int i = 0; i < Entries.size() && i < 5; i++)
        {
            Text += QString("%1. %2  %3  %4\n")
                    .arg(i + 1)
                    .arg(Entries[i].Player_Name)
                    .arg(Leaderboard_Data::Format_Time(Entries[i].Time_Ms))
                    .arg(Entries[i].Result);
        }

        if(Course_Finished && Current_Story_Level < 10)
            Text += "\nPress E to start next level.";
        else
            Text += "\nPress E to restart the run.";

        painter.drawText(Panel.adjusted(28, 72, -28, -24),
                         Qt::AlignLeft | Qt::AlignTop,
                         Text);

        painter.restore();
    }

    void Single_Player_Game_Screen::Update_Game()
    {
        float dt = 0.0085f;
        // Fallback floor stays at the bottom of the level so platforms handle the actual landing.
        float platformY = Current_Level.Get_Level_Height();

        const float Standing_Width = Story_Player.Get_Player_World_Size() * 0.42f;
        const float Standing_Height = Story_Player.Get_Player_World_Size() * 0.48f;
        Object_Physics_Body &Player_Body = Story_Player.Get_Player_Physics_Body();
        const float Stand_X = Player_Body.Get_X_Pos();
        const float Stand_Y = Player_Body.Get_Bottom() - Standing_Height;
        const bool Can_Stand_Up = !Current_Level.Is_Rect_Colliding_With_Platform(Stand_X,
                                                                                  Stand_Y,
                                                                                  Standing_Width,
                                                                                  Standing_Height);
        Story_Player.Set_Can_Stand_Up(Can_Stand_Up);

        if(Damage_Cooldown_Ticks > 0)
            Damage_Cooldown_Ticks--;

        if(Course_Running == false && Interact_Pressed == true)
        {
            if(Course_Finished == true && Tutorial_Mode == false && Current_Story_Level < 10)
                Advance_To_Next_Story_Level();
            else
                Start_Single_Player_Level(Tutorial_Mode);

            Interact_Pressed = false;
        }

        if(Course_Running == true)
        {
            //fisika
            Story_Player.Update_Story_Mode_Player(dt);
            //fisika
            Current_Level.Handle_Player_Collision(Story_Player.Get_Player_Physics_Body());

            if(Tutorial_Mode == true)
            {
                for(int i = 0; i < Tutorial_Enemies.size(); i++)
                {
                    Tutorial_Enemies[i].Update(dt, Story_Player);
                    Current_Level.Handle_Player_Collision(Tutorial_Enemies[i].Get_Physics_Body());
                    Tutorial_Enemies[i].Get_Physics_Body().Handle_Collision(0,
                                                                            platformY,
                                                                            Current_Level.Get_Level_Width(),
                                                                            Current_Level.Get_Tile_Size());
                }
            }

            Check_Win_Lose_Conditions();
            //fisika
            if(Course_Running == true)
                Story_Player.Handle_Story_Mode_Floor(platformY);
        }

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

    void Single_Player_Game_Screen::Check_Win_Lose_Conditions()
    {
        Object_Physics_Body &Body = Story_Player.Get_Player_Physics_Body();
        QRectF Player_Rect(Body.Get_X_Pos(), Body.Get_Y_Pos(), Body.Get_Width(), Body.Get_Height());

        QRectF Finish_Rect(Current_Level.Get_Finish_Pos().x(),
                           Current_Level.Get_Finish_Pos().y(),
                           Current_Level.Get_Tile_Size(),
                           Current_Level.Get_Tile_Size());

        if(Player_Rect.intersects(Finish_Rect))
        {
            Final_Time_Ms = (int)Course_Timer.elapsed();
            Total_Run_Time_Ms += Final_Time_Ms;
            Course_Running = false;
            Course_Finished = true;
            Leaderboard_Data::Add_Entry("Player", Total_Run_Time_Ms, "Win");
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
                Damage_Player_And_Respawn();
                return;
            }
        }

        if(Body.Get_Bottom() > Current_Level.Get_Level_Height())
        {
            Final_Time_Ms = (int)Course_Timer.elapsed();
            Total_Run_Time_Ms += Final_Time_Ms;
            Course_Running = false;
            Course_Lost = true;
            Leaderboard_Data::Add_Entry("Player", Total_Run_Time_Ms, "Lose");
        }
    }

    void Single_Player_Game_Screen::Advance_To_Next_Story_Level()
    {
        Current_Story_Level++;
        Current_Level.Load_Single_Player_Level_Number(Current_Story_Level);
        Tutorial_Enemies.clear();
        Story_Player.Set_Player_Spawn(Current_Level.Get_Player_Spawn());
        Camera_X = 0;
        Camera_Y = 0;
        Final_Time_Ms = 0;
        Course_Timer.restart();
        Course_Running = true;
        Course_Finished = false;
        Course_Lost = false;
        Damage_Cooldown_Ticks = 0;
    }

    void Single_Player_Game_Screen::Damage_Player_And_Respawn()
    {
        if(Damage_Cooldown_Ticks > 0)
            return;

        Player_Lives--;
        Damage_Cooldown_Ticks = 90;

        if(Player_Lives <= 0)
        {
            Final_Time_Ms = (int)Course_Timer.elapsed();
            Total_Run_Time_Ms += Final_Time_Ms;
            Course_Running = false;
            Course_Lost = true;
            Leaderboard_Data::Add_Entry("Player", Total_Run_Time_Ms, "Lose");
            return;
        }

        // leave space here for enemy hitboxes to call this same damage function later
        Story_Player.Set_Player_Spawn(Current_Level.Get_Player_Spawn());
    }

    void Single_Player_Game_Screen::Load_Tutorial_Enemies()
    {
        Tutorial_Enemies.clear();

        QVector<QPointF> Enemy_Spawns = Current_Level.Get_Enemy_Spawns();

        for(int i = 0; i < Enemy_Spawns.size(); i++)
        {
            enemy New_Enemy(enemy::Little);
            New_Enemy.Set_Spawn(Enemy_Spawns[i]);
            Tutorial_Enemies.append(New_Enemy);
        }
    }
