#ifndef MENU_MULTIPLAYER_SCREEN_H
#define MENU_MULTIPLAYER_SCREEN_H

#include <QWidget>

class QPushButton;

class Menu_Multiplayer_Screen : public QWidget
{
public:
    explicit Menu_Multiplayer_Screen(QWidget *parent = nullptr);

    QPushButton *Get_Host_Button() const;
    QPushButton *Get_Client_Button() const;
    QPushButton *Get_Back_Button() const;

private:
    QPushButton *Host_Button;
    QPushButton *Client_Button;
    QPushButton *Back_Button;
};

#endif // MENU_MULTIPLAYER_SCREEN_H
