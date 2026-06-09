#ifndef MENU_JOIN_SERVER_SCREEN_H
#define MENU_JOIN_SERVER_SCREEN_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QListWidget>
#include <QRegularExpressionValidator>
#include <QLabel>
#include <QVBoxLayout>
#include <QResizeEvent>
#include <QStringList>

class Menu_Join_Server_Screen : public QWidget
{
    Q_OBJECT

public:
    explicit Menu_Join_Server_Screen(QWidget *parent = nullptr);

    // expose controls the menu manager needs for join flow
    void Set_Lobby_List(const QStringList &lobbyTexts);

signals:
    void Join_Button_Clicked(int selectedRow, QString playerName);
    void Refresh_Button_Clicked();
    void Back_Button_Clicked();

protected:
    // resize fonts to match the current window width
    void resizeEvent(QResizeEvent *event) override;

private:
    void Apply_Responsive_Styles();

    // layout and join widgets
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

private slots:
    void Handle_Join_Button_Clicked();
    void Set_Button_Cursor_To_Closed_Hand();
    void Set_Button_Cursor_To_Pointing_Hand();
};

#endif // MENU_JOIN_SERVER_SCREEN_H
