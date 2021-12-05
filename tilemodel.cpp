#include "tilemodel.h"
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include <QTime>

TileModel::TileModel(QObject *parent) :
    QObject(parent)
{
}

void TileModel::adjustNodes()
{
    bool last = this->nodes[3];
    this->nodes.pop_back();
    this->nodes.insert(this->nodes.cbegin(), last);
}

void TileModel::adjustNodes(int times)
{
    for (int i = 0; i < times; ++i)
        this->adjustNodes();
}

QString TileModel::getNodeString()
{

    QString nodeString;
    for (int i = 0; i < 4; ++i) {
        if (this->nodes[i])
            nodeString += QString::number(i);
    }
    return nodeString;
}

void TileModel::rotate90()
{
    this->adjustNodes();
    this->rotateAngle += 90;
}

