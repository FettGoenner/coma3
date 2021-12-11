#include <QPainter>
#include <QTimer>

#include "cornertilemodel.h"

CornerTile::CornerTile(QObject *parent) :
    TileModel(parent)
{
    // default CornerTile
    this->east = TileModel::ON;
    this->south = TileModel::ON;
}

CornerTile::CornerTile(const QVector<bool>& tile, QObject *parent):
    TileModel(parent)
{
    CornerTile::setNodes(tile);
}

void CornerTile::setNodes(const QVector<bool>& tile)
{
    // set default nodes if tile is invalid.
    if (tile.size() == 0 ||
            !(tile[TileModel::North] || tile[TileModel::East] || tile[TileModel::South] || tile[TileModel::West])) {
        this->east = TileModel::ON;
        this->south = TileModel::ON;
        return;
    }

    this->clearNodes();
    TileModel::setNodes(tile);

    // get rotateAngle
    if (this->south && this->west)
        this->rotateAngle = 90;
    else if (this->west && this->north)
        this->rotateAngle = 180;
    else if (this->north && this->east)
        this->rotateAngle = 270;
    else
        this->rotateAngle = 0;

    emit this->nodesChanged();
}



