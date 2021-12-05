#include "linetilemodel.h"
#include <QPainter>

LineTile::LineTile(QObject *parent) :
    TileModel(parent)
{
    this->nodes[0] = true;
    this->nodes[2] = true;
}

LineTile::LineTile(QString nodes, QObject *parent):
    TileModel(parent)
{
    LineTile::setNodes(nodes);
}

void LineTile::setNodes(QString nodes)
{
    this->clearNodes();
    if (nodes.isEmpty()) {
        this->nodes[0] = true;
        this->nodes[2] = true;
    } else if (nodes.size() == 2) {
        for (const QChar &s : nodes)
            this->nodes[s.digitValue()] = true;
    } else {
        throw "The nodes do not match to any LineTiles";
    }
    if (!this->nodes[0]){
        if (this->rotateAngle % 360 != 270)
            this->rotateAngle = 90;
    }
    else {
        if (this->rotateAngle % 360 != 180)
            this->rotateAngle = 0;
    }

    emit this->nodesChanged();
}

