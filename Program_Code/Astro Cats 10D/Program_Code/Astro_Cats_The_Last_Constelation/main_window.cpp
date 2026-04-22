#include "main_window.h"
#include "menu_maker.h"

#include <QStackedWidget>

Main_Window::Main_Window(QWidget *parent)
    : QMainWindow(parent)
{
    // Creating the screen Stack
    ScreenStack = new QStackedWidget(this);

    // Creating the screens
    Main_Menu = new Menu_Maker(this);

    // Add the Main Menu to the Screen Stack
    ScreenStack->addWidget(Main_Menu);

    //Set the Screen Stack as the sentral widget
    setCentralWidget(ScreenStack);

    //Starting with the main menu
    ScreenStack->setCurrentWidget(Main_Menu);

    //View it fullscreen
    showFullScreen();

}

Main_Window::~Main_Window()
{
}

