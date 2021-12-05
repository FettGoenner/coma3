#include "endtilemodel.h"
#include <QPainter>

EndTile::EndTile(QObject *parent) :
    TileModel(parent)
{
    this->nodes[0] = true;
}

EndTile::EndTile(QString nodes, QObject *parent):
    TileModel(parent)
{
    EndTile::setNodes(nodes);
}

void EndTile::setNodes(QString nodes)
{
    this->clearNodes();
    if (nodes.isEmpty()) {
        this->nodes[0] = true;
    } else if (nodes.size() == 1) {
        this->nodes[nodes.toInt()] = true;
    } else {
        throw "The nodes does not match to any EndTiles";
    }
    this->rotateAngle = nodes.toInt()*90;

    emit this->nodesChanged();
}


