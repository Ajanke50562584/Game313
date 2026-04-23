#ifndef MENU_MAIN_SCREEN_H
#define MENU_MAIN_SCREEN_H

#include <QWidget>
#include <QList>

class QLabel;
class QPushButton;
class QVBoxLayout;
class QResizeEvent;

class Menu_Main_Screen : public QWidget
{
public:
    explicit Menu_Main_Screen(QWidget *parent = nullptr);

    QPushButton *Get_Multiplayer_Button() const;

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QLabel *Game_Title;
    QVBoxLayout *Main_Layout;
    QList<QPushButton*> Buttons;
    QPushButton *Multiplayer_Button;
};

#endif // MENU_MAIN_SCREEN_H
