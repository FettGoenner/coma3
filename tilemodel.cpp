#include "tilemodel.h"
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include <QTime>

TileModel::TileModel(QObject *parent) :
    QObject(parent)
{
}

void TileModel::adjustNodes(int times)
{
    for (int i = 0; i < times; ++i) {
        bool temp = this->north;
        this->north = this->west;
        this->west  = this->south;
        this->south = this->east;
        this->east  = temp;
    }
}

QVector<bool> TileModel::getNodeVector()
{
    return {this->north, this->east, this->south, this->west};
}

void TileModel::rotate90()
{
    this->adjustNodes();
    this->rotateAngle += 90;
}

QString TileModel::getNodeString()
{
    QString nodeString = "";
    if (this->north)
        nodeString += "0";
    if (this->east)
        nodeString += "1";
    if (this->south)
        nodeString += "2";
    if (this->west)
        nodeString += "3";
    return nodeString;
}

QString TileModel::getNodeString(const QVector<bool>& tile)
{
    QString nodeString = "";
    if (tile[TileModel::North])
        nodeString += "0";
    if (tile[TileModel::East])
        nodeString += "1";
    if (tile[TileModel::South])
        nodeString += "2";
    if (tile[TileModel::West])
        nodeString += "3";
    return nodeString;
}


QString TileModel::getTileTypeByVector(const QVector<bool>& tile)
{
    QString nodeString = TileModel::getNodeString(tile);
    if (nodeString.size() == 1) {
        return "EndTile";
    } else if (nodeString.size() == 3) {
        return "JunctionTile";
    } else {
        int first = nodeString[0].digitValue(), second = nodeString[1].digitValue();
        if (first > second) {
            int temp = first;
            first = second;
            second = temp;
        }
        if (second - first == 1 || second - first == 3)
            return "CornerTile";

        else
            return "LineTile";
    }
    return "Tile";
}

size_t TileModel::countNodes(const QVector<bool>& tile)
{
    int count = 0;
    for (const bool &node : tile)
        if (node)
            ++count;
    return count;
}



