#ifndef MENU_MULTIPLAYER_SCREEN_H
#define MENU_MULTIPLAYER_SCREEN_H

#include <QWidget>
#include <QLabel>

class QPushButton;

class Menu_Multiplayer_Screen : public QWidget
{
public:
    explicit Menu_Multiplayer_Screen(QWidget *parent = nullptr);

    QPushButton *Get_Host_Button() const;
    QPushButton *Get_Client_Button() const;
    QPushButton *Get_Back_Button() const;

private:
    int Current_Frame;
    QPushButton *Host_Button;
    QPushButton *Client_Button;
    QPushButton *Back_Button;
    QLabel *Character_Label;
    QList<QPixmap> Frames;
    QTimer *Multiplayer_Character_Timer;
    QPixmap *Sprite_Sheet;

   void resizeEvent(QResizeEvent *event);

private slots:
    void Update_Frame();
};

#endif // MENU_MULTIPLAYER_SCREEN_H
