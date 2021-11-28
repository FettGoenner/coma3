#include "tile.h"
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>

Tile::Tile(QFrame *parent) :
    QFrame(parent)
{
    setCursor(Qt::PointingHandCursor);
    setStyleSheet("QWidget{"
                  "border:2px solid gray;"
                  "}");
}

void Tile::rotate90()
{
    bool last = nodes[3];
    nodes.pop_back();
    nodes.insert(nodes.begin(), last);
    rotateAngle == 270 ? rotateAngle = 0 : rotateAngle += 90;
    update();
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
    rotate90();
    // send signal clicked()
    emit clicked();
    return QWidget::mousePressEvent(ev);
}
