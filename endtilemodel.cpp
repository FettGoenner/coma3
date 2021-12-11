#include "endtilemodel.h"
#include <QPainter>

EndTile::EndTile(QObject *parent) :
    TileModel(parent)
{
    this->north = TileModel::ON;
}

EndTile::EndTile(const QVector<bool>& tile, QObject *parent):
    TileModel(parent)
{
    EndTile::setNodes(tile);
}

void EndTile::setNodes(const QVector<bool>& tile)
{
    if (tile.size() == 0 ||
      !(tile[TileModel::North] || tile[TileModel::East] || tile[TileModel::South] || tile[TileModel::West])) {
        this->north = TileModel::ON;
        return;
    }
    this->clearNodes();
    TileModel::setNodes(tile);
//    if (!EndTile::isValidTile())
//        throw "The nodes does not match to any EndTiles";

    if (this->east)
        this->rotateAngle = 90;
    else if (this->south)
        this->rotateAngle = 180;
    else if (this->west)
        this->rotateAngle = 270;
    else
        this->rotateAngle = 0;

    emit this->nodesChanged();
}

//bool EndTile::isValidTile()
//{
//    QString nodeString = this->getNodeString();
//    if (nodeString.size() == 1)
//        return true;

//    return false;
//}


