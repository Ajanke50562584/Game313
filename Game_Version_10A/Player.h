#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QObject>
#include <QKeyEvent>
#include <QTimer>



class Player: public QObject, public QGraphicsPixmapItem
{Q_OBJECT
public:
    Player (QGraphicsItem * parent=0);

    QTimer * timer;

    void keyPressEvent (QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent * event);

private:
    const int playerSpeed = 5;
    int moveState;
    void movePlayer(int moveState);

public slots: //might need to be public?
    void timerTick();


};

#endif // PLAYER_H
