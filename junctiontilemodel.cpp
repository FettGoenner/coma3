#include "junctiontilemodel.h"
#include <QPainter>

JunctionTile::JunctionTile(QObject *parent) :
    TileModel(parent)
{
    // default CornerTile
    this->east = TileModel::ON;
    this->south = TileModel::ON;
    this->west = TileModel::ON;
}

JunctionTile::JunctionTile(const QVector<bool>& tile, QObject *parent):
    TileModel(parent)
{
    JunctionTile::setNodes(tile);
}

void JunctionTile::setNodes(const QVector<bool>& tile)
{
    // set default nodes if tile is invalid.
    if (tile.size() == 0 ||
      !(tile[TileModel::North] || tile[TileModel::East] || tile[TileModel::South] || tile[TileModel::West])) {
        this->east = TileModel::ON;
        this->south = TileModel::ON;
        this->west = TileModel::ON;
        return;
    }
    this->clearNodes();
    TileModel::setNodes(tile);

    // get rotateAngle
    if (!this->north)
        this->rotateAngle = 0;
    else if (!this->east)
        this->rotateAngle = 90;
    else if (!this->south)
        this->rotateAngle = 180;
    else
        this->rotateAngle = 270;

    emit this->nodesChanged();
}

//bool JunctionTile::isValidTile()
//{
//    QString nodeString = this->getNodeString();
//    if (nodeString.size() == 3)
//        return true;

//    return false;
//}
