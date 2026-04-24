#ifndef MENU_CREATE_LOBBY_SCREEN_H
#define MENU_CREATE_LOBBY_SCREEN_H

#include <QWidget>

class QPushButton;
class QLineEdit;
class QCheckBox;
class QSpinBox;
class QRegularExpressionValidator;
class QLabel;
class QVBoxLayout;
class QResizeEvent;

class Menu_Create_Lobby_Screen : public QWidget
{
public:
    explicit Menu_Create_Lobby_Screen(QWidget *parent = nullptr);

    QPushButton *Get_Create_Lobby_Button() const;
    QPushButton *Get_Back_Button() const;
    QLineEdit *Get_Lobby_Name_Input() const;
    QSpinBox *Get_Max_Players_Input() const;
    QCheckBox *Get_Public_Lobby_Checkbox() const;
    QLineEdit *Get_Password_Input() const;

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QVBoxLayout *Main_Layout;
    QWidget *Card_Widget;
    QVBoxLayout *Card_Layout;
    QLabel *Title_Label;
    QLabel *Sub_Title_Label;
    QLabel *Lobby_Name_Label;
    QLabel *Max_Players_Label;
    QLabel *Password_Label;
    QLineEdit *Lobby_Name_Input;
    QRegularExpressionValidator *Lobby_Name_Validator;
    QSpinBox *Max_Players_Input;
    QCheckBox *Public_Lobby_Checkbox;
    QLineEdit *Password_Input;
    QCheckBox *Show_Password_Checkbox;
    QPushButton *Create_Lobby_Button;
    QPushButton *Back_Button;
};

#endif // MENU_CREATE_LOBBY_SCREEN_H
