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

    // menu buttons for navigation
    QPushButton *Get_Host_Button() const;
    QPushButton *Get_Client_Button() const;
    QPushButton *Get_Back_Button() const;

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
    void Set_Button_Cursor_To_Closed_Hand();
    void Set_Button_Cursor_To_Pointing_Hand();
};

#endif // MENU_MULTIPLAYER_SCREEN_H
