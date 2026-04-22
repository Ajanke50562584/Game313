#include "Game.h"
//#include "Player.h"
//#include <QTimer>

Game::Game(QWidget *parent)
{

    //create a new scene
    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,1900,1000);
    setScene(scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(1900,1000);

    //Create the player
    player = new Player();

    scene->addItem(player);
    player->setFlag(QGraphicsItem::ItemIsFocusable);
    player->setFocus();


    show();

}
