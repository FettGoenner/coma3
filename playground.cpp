#include "playground.h"
#include "straightnode.h"
#include "turnnode.h"
#include <QVector>

PlayGround::PlayGround(QWidget *parent) :
    QWidget(parent)
  , gameStarted(false)
  , totalPlayTime(0)
  , totalSteps(0)
{
    // test
    StraightNode * sNode = new StraightNode(Qt::blue);
    sNode->setParent(this);
    sNode->resize(200, 200);
    sNode->move(0, 200);
    TurnNode * tNode = new TurnNode(Qt::blue);
    tNode->resize(200, 200);
    tNode->setParent(this);
    connect(tNode, &Node::clicked, this, &PlayGround::setSteps);
    connect(sNode, &Node::clicked, this, &PlayGround::setSteps);
}
