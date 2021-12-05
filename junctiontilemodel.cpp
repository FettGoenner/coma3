#include "junctiontilemodel.h"
#include <QPainter>

JunctionTile::JunctionTile(QObject *parent) :
    TileModel(parent)
{
    this->nodes[1] = true;
    this->nodes[2] = true;
    this->nodes[3] = true;
}

JunctionTile::JunctionTile(QString nodes, QObject *parent):
    TileModel(parent)
{
    JunctionTile::setNodes(nodes);
}

void JunctionTile::setNodes(QString nodes)
{
    this->clearNodes();
    if (nodes.isEmpty()) {
        this->nodes[1] = true;
        this->nodes[2] = true;
        this->nodes[3] = true;
    } else if (nodes.size() == 3) {
        for (const QChar &s : nodes)
            this->nodes[s.digitValue()] = true;
    } else {
        throw "The nodes does not match to any JunctionTiles";
    }
    for (int i = 0; i < 4; ++i) {
        if (!this->nodes[i]) {
            this->rotateAngle = i*90;
            break;
        }
    }

    emit this->nodesChanged();
}
