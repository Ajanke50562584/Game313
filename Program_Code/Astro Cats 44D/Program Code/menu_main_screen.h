#ifndef MENU_MAIN_SCREEN_H
#define MENU_MAIN_SCREEN_H

#include <QWidget>
#include <QList>

class QLabel;
class QPushButton;
class QVBoxLayout;
class QHideEvent;
class QResizeEvent;
class QShowEvent;

class Menu_Main_Screen : public QWidget
{
public:
    explicit Menu_Main_Screen(QWidget *parent = nullptr);

    // multiplayer button for menu navigation
    QPushButton *Get_Multiplayer_Button() const;

protected:
    // resize fonts and character placement with the window
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;

private:
    // title screen layout and animation data
    int Current_Frame;
    int Completed_Animation_Loops;
    bool Animation_Status_Reported;
    QLabel *Game_Title;
    QVBoxLayout *Main_Layout;
    QList<QPushButton*> Buttons;
    QPushButton *Multiplayer_Button;
    QLabel *Character_Label;
    QList<QPixmap> Frames;
    QTimer *Menu_Character_Timer;
    QPixmap *Sprite_Sheet;

    void Apply_Responsive_Sizing();
    void Report_Animation_Error_Once();

private slots:
    // advance the menu animation
    void Update_Frame();
};

#endif // MENU_MAIN_SCREEN_H
