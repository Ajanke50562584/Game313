#ifndef MENU_MULTIPLAYER_SCREEN_H
#define MENU_MULTIPLAYER_SCREEN_H

#include <QWidget>
#include <QLabel>

class QPushButton;
class QHideEvent;
class QResizeEvent;
class QShowEvent;

class Menu_Multiplayer_Screen : public QWidget
{
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
    void Report_Animation_Error_Once();

protected:
    // keep the character anchored during resize
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;

private slots:
    // advance the idle animation
    void Update_Frame();
};

#endif // MENU_MULTIPLAYER_SCREEN_H
