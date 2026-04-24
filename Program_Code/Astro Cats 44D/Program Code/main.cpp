#include "main_window.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    // start the app and open the main window
    QApplication a(argc, argv);
    Main_Window w;
    w.show();
    return a.exec();
}
