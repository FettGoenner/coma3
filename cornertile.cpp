#include "cornertile.h"

CornerTile::CornerTile(QObject *parent) : TileModel(parent)
{ setNodes({0,1,1,0}); }

CornerTile::CornerTile(const QVector<bool>& vec, QObject *parent)
    :TileModel({0,1,1,0},parent)
{
    if (TileModel::typeByVector(vec) != TileModel::CornerTile)
        throw "CornerTile is invalid!";
    int count = 0;
    if (vec[2] && vec[3])
        count = 1;
    else if (vec[3] && vec[0])
        count = 2;
    else if (vec[0] && vec[1])
        count = 3;
    rotate(count);
}
