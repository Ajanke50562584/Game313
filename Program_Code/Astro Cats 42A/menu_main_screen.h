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
    int Current_Frame;
    QLabel *Game_Title;
    QVBoxLayout *Main_Layout;
    QList<QPushButton*> Buttons;
    QPushButton *Multiplayer_Button;
    QLabel *Character_Label;
    QList<QPixmap> Frames;
    QTimer *Menu_Character_Timer;
    QPixmap *Sprite_Sheet;

private slots:
    void Update_Frame();
};

#endif // MENU_MAIN_SCREEN_H
