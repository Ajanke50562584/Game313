#include "Player.h"
#include <QtDebug>


Player::Player(QGraphicsItem *parent) : QGraphicsPixmapItem(parent)
{
    //set graphic
    setPixmap(QPixmap(":/images/CatA.png"));
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

    //initialization
    moveState = 0;
    //timer
    timer = new QTimer(this);
    //timer->setInterval(20);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerTick()));
    timer->start(20);
}

void Player::keyPressEvent(QKeyEvent *event)
{
    qDebug() << event->key();

    switch(event->key())  //maybe change to andreas' method of movement since we can set speed???? no just in general it is necessary =(
    {
    //move left
    case Qt::Key_Left:
    case Qt::Key_A:
        //setPos(x()-10,y());
        moveState = -1;
        qDebug() << "left";
        break;
    //move right
    case Qt::Key_Right:
    case Qt::Key_D:
        //setPos(x()+10,y());
        moveState = 1;
        qDebug() << "right";
        break;



    }

    //qDebug()    << "Key pressed.";

}

void Player::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key())  //maybe change to andreas' method of movement since we can set speed????
    {
    //move left
    case Qt::Key_Left:
    case Qt::Key_A:
    case Qt::Key_Right:
    case Qt::Key_D:
        moveState = 0;
        qDebug() << "stop";

        break;
    }
}

void Player::timerTick()
{
    qDebug() << "Tick";
    movePlayer(moveState);
}

void Player::movePlayer(int moveState)
{
    bool move = false;
    if ((moveState == -1) && (this->x() >= 0))
    {
        move = true;
        qDebug() << "Move";
    }
        else if ((moveState == 1) && (this->x() <= 1000)) //change to width of scene
    {
        move = true;
        qDebug() << "Move";
    }


    if (move)
        setPos(x()+ moveState*playerSpeed,y());

}
