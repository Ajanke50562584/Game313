#ifndef MENU_MULTIPLAYER_SCREEN_H
#define MENU_MULTIPLAYER_SCREEN_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHideEvent>
#include <QResizeEvent>
#include <QShowEvent>
#include <QTimer>
#include <QPixmap>

class Menu_Multiplayer_Screen : public QWidget
{
    Q_OBJECT

public:
    explicit Menu_Multiplayer_Screen(QWidget *parent = nullptr);

signals:
    // menu button signals for navigation
    void Host_Button_Clicked();
    void Client_Button_Clicked();
    void Back_Button_Clicked();

private:
    // button refs and animation data
    int Current_Frame;
    int Completed_Animation_Loops;
    bool Animation_Status_Reported;

    QPushButton *Host_Button;
    QPushButton *Client_Button;
    QPushButton *Back_Button;

    QLabel *Character_Label;
    QList<QPixmap> Frames;
    QTimer *Multiplayer_Character_Timer;
    QPixmap *Sprite_Sheet;

    void Apply_Responsive_Sizing();
    void Apply_Responsive_Styles();
    void Report_Animation_Error_Once();

protected:
    // keep the character anchored during resize
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;

private slots:
    // advance the idle animation
    void Update_Frame();
    void Set_Button_Cursor_To_Closed_Hand(); // Just 1 awesomeness point my good sir G_g
    void Set_Button_Cursor_To_Pointing_Hand();

};

#endif // MENU_MULTIPLAYER_SCREEN_H
