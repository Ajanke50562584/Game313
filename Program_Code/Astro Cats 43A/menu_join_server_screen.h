#ifndef MENU_JOIN_SERVER_SCREEN_H
#define MENU_JOIN_SERVER_SCREEN_H

#include <QWidget>

class QPushButton;
class QLineEdit;
class QListWidget;
class QRegularExpressionValidator;
class QLabel;
class QVBoxLayout;
class QResizeEvent;

class Menu_Join_Server_Screen : public QWidget
{
public:
    explicit Menu_Join_Server_Screen(QWidget *parent = nullptr);

    QPushButton *Get_Join_Button() const;
    QPushButton *Get_Refresh_Button() const;
    QPushButton *Get_Back_Button() const;
    QListWidget *Get_Lobby_List() const;
    QLineEdit *Get_Player_Name_Input() const;

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QVBoxLayout *Main_Layout;
    QWidget *Card_Widget;
    QVBoxLayout *Card_Layout;
    QLabel *Title_Label;
    QLabel *Sub_Title_Label;
    QLabel *Player_Name_Label;
    QLineEdit *Player_Name_Input;
    QRegularExpressionValidator *Player_Name_Validator;
    QListWidget *Lobby_List;
    QPushButton *Join_Button;
    QPushButton *Refresh_Button;
    QPushButton *Back_Button;
};

#endif // MENU_JOIN_SERVER_SCREEN_H
