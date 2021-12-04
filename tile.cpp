#include "tile.h"
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include <QTime>

Tile::Tile(QFrame *parent) :
    QFrame(parent)
  ,animationTimer(new QTimer)
{
    setCursor(Qt::PointingHandCursor);
    setStyleSheet("QWidget{"
                  "border:2px solid gray;"
                  "}");

    this->animationTimer->setInterval(10);
    connect(this->animationTimer, &QTimer::timeout, this, &Tile::rotateTimeout);
}

void Tile::rotate90()
{
    this->adjustNodes();
    this->rotateAngle += 90;
    update();
}

void Tile::rotateWithAnimation(int angle)
{
    if (angle == 0)
        return;
    this->animationAngele = angle;

    this->animationTimer->start();
    for (int i = 0; i < angle/90; ++i) {
        this->adjustNodes();
    }
}

void Tile::adjustNodes()
{
    bool last = this->nodes[3];
    this->nodes.pop_back();
    this->nodes.insert(this->nodes.cbegin(), last);
}

QString Tile::getNodeString()
{

    QString nodeString;
    for (int i = 0; i < 4; ++i) {
        if (this->nodes[i])
            nodeString += QString::number(i);
    }
    return nodeString;
}

void Tile::mouseReleaseEvent(QMouseEvent *ev)
{
    if (!this->animationTimer->isActive()){
        this->rotateWithAnimation(90);
        // send signal clicked()
        emit clicked();
    }
    return QWidget::mousePressEvent(ev);
}

void Tile::rotateTimeout()
{
    this->rotateAngle += 10;
    this->animationAngele -= 10;
    update();
    if (this->animationAngele == 0){
        this->animationAngele = 90;
        this->animationTimer->stop();
    }
}
