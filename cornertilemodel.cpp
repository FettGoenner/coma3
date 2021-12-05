#include "cornertilemodel.h"
#include <QPainter>
#include <QTimer>

CornerTile::CornerTile(QObject *parent) :
    TileModel(parent)
{
    this->nodes[1] = true;
    this->nodes[2] = true;
}

CornerTile::CornerTile(QString nodes, QObject *parent):
    TileModel(parent)
{
    CornerTile::setNodes(nodes);
}

void CornerTile::setNodes(QString nodes)
{
    this->clearNodes();
    if (nodes.isEmpty()) {
        this->nodes[1] = true;
        this->nodes[2] = true;
    } else if (nodes.size() == 2) {
        int first = nodes[0].digitValue(), second = nodes[1].digitValue();
        if (first > second) {
            int temp = first;
            first = second;
            second = temp;
        }
        if (second - first == 1 || second - first == 3) {
            this->nodes[first] = true;
            this->nodes[second] = true;
        } else
            throw "The nodes does not match to any CornerTiles";

    } else {
        throw "The nodes does not match to any CornerTiles";
    }
    int index1 = 1, index2 = 2;
    this->rotateAngle = 0;
    for (int i = 0; i < 4; ++i) {
        if (this->nodes[index1] && this->nodes[index2])
            break;
        ++index1, ++index2;
        this->rotateAngle += 90;
        if (index1 == 4)
            index1 = 0;
        if (index2 == 4)
            index2 = 0;
    }

    emit this->nodesChanged();
}

