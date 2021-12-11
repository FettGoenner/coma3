#include "linetilemodel.h"
#include <QPainter>

LineTile::LineTile(QObject *parent) :
    TileModel(parent)
{
    // default CornerTile
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
    // set default nodes if tile is invalid.
    if (tile.size() == 0 ||
            !(tile[TileModel::North] || tile[TileModel::East] || tile[TileModel::South] || tile[TileModel::West])) {
        this->north = TileModel::ON;
        this->south = TileModel::ON;
        return;
    }
    this->clearNodes();
    TileModel::setNodes(tile);

    // get rotateAngle
    if (!this->north){
        if (this->rotateAngle % 360 != 270)
            this->rotateAngle = 90;
    } else {
        if (this->rotateAngle % 360 != 180)
            this->rotateAngle = 0;
    }

    emit this->nodesChanged();
}

