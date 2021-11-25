#include "playground.h"
#include "straightnode.h"
#include "turnnode.h"
#include "headnode.h"
#include <QVector>
#include <QDebug>
#include <QPainter>
#include <QPen>
#include <QGridLayout>

PlayGround::PlayGround(QWidget *parent,int playGroundSize) :
    QWidget(parent)
  , gameStarted(false)
  , totalPlayTime(0)
  , totalSteps(0)
  , playGroundSize(playGroundSize) // default is 7 -> 7x7
  , playGround(QVector<QVector<Node*>>(playGroundSize, QVector<Node*>(playGroundSize, nullptr)))

{
    // test
//    StraightNode * sNode = new StraightNode(Qt::blue);
//    sNode->setParent(this);
//    sNode->resize(200, 200);
//    sNode->move(0, 200);
//    TurnNode * tNode = new TurnNode(Qt::blue);
//    tNode->resize(200, 200);
//    tNode->setParent(this);
//    connect(tNode, &Node::clicked, this, &PlayGround::setSteps);
//    connect(sNode, &Node::clicked, this, &PlayGround::setSteps);
    QGridLayout *layout = new QGridLayout(this);
    layout->setSpacing(0);
    setLayout(layout);
    for (int i = 0; i < playGroundSize; ++i) {
        for (int j = 0; j < playGroundSize; ++j) {
            TurnNode * node = new TurnNode(Qt::blue);
            node->setParent(this);
            playGround[i][j] = node;
            layout->addWidget(node, i, j);
            connect(node, &Node::clicked, this, &PlayGround::setSteps);
        }
    }

}

void PlayGround::paintEvent(QPaintEvent *ev)
{
    int width = this->width(), height = this->height();
    // draw the deflaut StraightNode, from top to bottom
    QPainter painter(this);

    QPen pen;
    pen.setColor(QColor(255, 0, 0, 0));

    painter.setPen(pen);
    QBrush brush;
    brush.setColor(Qt::white);
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);
    painter.drawRect(0, 0, width, height);

    return QWidget::paintEvent(ev);
}
