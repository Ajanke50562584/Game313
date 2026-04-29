#ifndef MENU_CREATE_LOBBY_SCREEN_H
#define MENU_CREATE_LOBBY_SCREEN_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QRegularExpressionValidator>
#include <QLabel>
#include <QVBoxLayout>
#include <QResizeEvent>

class Menu_Create_Lobby_Screen : public QWidget
{
    Q_OBJECT

public:
    explicit Menu_Create_Lobby_Screen(QWidget *parent = nullptr);

    // expose the form controls the menu manager needs
    QPushButton *Get_Create_Lobby_Button() const;
    QPushButton *Get_Back_Button() const;
    QLineEdit *Get_Lobby_Name_Input() const;
    QLineEdit *Get_Host_Name_Input() const;
    QSpinBox *Get_Max_Players_Input() const;
    QCheckBox *Get_Public_Lobby_Checkbox() const;

protected:
    // resize fonts to fit the current window size
    void resizeEvent(QResizeEvent *event) override;

private:
    void Apply_Responsive_Styles();
    void Update_Create_Button_State();

private slots:
    void Handle_Host_Name_Changed(const QString &text);
    void Handle_Public_Lobby_Toggled(bool checked);
    void Handle_Show_Password_Toggled(bool checked);
    void Set_Button_Cursor_To_Closed_Hand();
    void Set_Button_Cursor_To_Pointing_Hand();

private:
    // layout and form widgets
    QVBoxLayout *Main_Layout;
    QWidget *Card_Widget;
    QVBoxLayout *Card_Layout;
    QLabel *Title_Label;
    QLabel *Sub_Title_Label;
    QLabel *Lobby_Name_Label;
    QLabel *Host_Name_Label;
    QLabel *Max_Players_Label;
    QLabel *Password_Label;
    QLineEdit *Lobby_Name_Input;
    QLineEdit *Host_Name_Input;
    QRegularExpressionValidator *Lobby_Name_Validator;
    QRegularExpressionValidator *Host_Name_Validator;
    QSpinBox *Max_Players_Input;
    QCheckBox *Public_Lobby_Checkbox;
    QLineEdit *Password_Input;
    QCheckBox *Show_Password_Checkbox;
    QPushButton *Create_Lobby_Button;
    QPushButton *Back_Button;
};

#endif // MENU_CREATE_LOBBY_SCREEN_H
