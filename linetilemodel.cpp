#include "linetilemodel.h"
#include <QPainter>

LineTile::LineTile(QObject *parent) :
    TileModel(parent)
{
    this->north = TileModel::ON;
    this->south = TileModel::ON;
}

LineTile::LineTile(const QVector<bool>& tile, QObject *parent):
    TileModel(parent)
{
    LineTile::setNodes(tile);
}

void LineTile::setNodes(const QVector<bool>& tile)
{
    if (tile.size() == 0 ||
            !(tile[TileModel::North] || tile[TileModel::East] || tile[TileModel::South] || tile[TileModel::West])) {
        this->north = TileModel::ON;
        this->south = TileModel::ON;
        return;
    }
    this->clearNodes();
    TileModel::setNodes(tile);
    if (!LineTile::isValidTile()) {
        throw "The nodes do not match to any LineTiles";
    }
    if (!this->north){
        if (this->rotateAngle % 360 != 270)
            this->rotateAngle = 90;
    } else {
        if (this->rotateAngle % 360 != 180)
            this->rotateAngle = 0;
    }

    emit this->nodesChanged();
}

bool LineTile::isValidTile()
{

    QString nodeString = this->getNodeString();
    if (nodeString.size() == 2) {
        int first = nodeString[0].digitValue(), second = nodeString[1].digitValue();
        if (second - first == 2 || second - first == -2)
            return true;
    }
    return false;
}

