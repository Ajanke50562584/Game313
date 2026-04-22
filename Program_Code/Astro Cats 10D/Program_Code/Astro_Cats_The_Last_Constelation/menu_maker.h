#ifndef MENU_MAKER_H
#define MENU_MAKER_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QResizeEvent>
#include <QList>
#include <QVBoxLayout>


class Menu_Maker : public QWidget
{
    Q_OBJECT

public:
    explicit Menu_Maker(QWidget *parent = nullptr);


protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QLabel *Game_Title;

    QVBoxLayout *Main_Layout;
    QList<QPushButton*> Buttons;


};

#endif // MENU_MAKER_H
