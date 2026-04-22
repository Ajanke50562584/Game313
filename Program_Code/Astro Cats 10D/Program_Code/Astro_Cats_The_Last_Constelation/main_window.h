#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

class Menu_Maker; // Just sothat I don't need to include the entire Menu_maker header T_T
class QStackedWidget;

class Main_Window : public QMainWindow
{
    Q_OBJECT

public:
    Main_Window(QWidget *parent = nullptr);

    ~Main_Window();

private:
    QStackedWidget *ScreenStack;

    Menu_Maker *Main_Menu;

};
#endif // MAIN_WINDOW_H
