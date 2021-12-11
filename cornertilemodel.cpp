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
//    if (!CornerTile::isValidTile())
//        throw "The nodes does not match to any CornerTiles";

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

//bool CornerTile::isValidTile()
//{
//    QString nodeString = this->getNodeString();
//    if (nodeString.size() == 2) {
//        int first = nodeString[0].digitValue(), second = nodeString[1].digitValue();
//        if (first > second) {
//            int temp = first;
//            first = second;
//            second = temp;
//        }
//        if (second - first == 1 || second - first == 3) {
//            return true;
//        }
//    }
//    return false;
//}


