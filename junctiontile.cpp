#include "junctiontile.h"

JunctionTile::JunctionTile(QObject *parent) : TileModel(parent)
{ setNodes({0,1,1,1}); }
JunctionTile::JunctionTile(const QVector<bool>& vec, QObject *parent)
    :TileModel({0,1,1,1},parent)
{
    if (TileModel::typeByVector(vec) != TileModel::JunctionTile)
        throw "JunctionTile is invalid!";

    for (int count = 0; count < 4; ++count)
        if (!vec[count])
            rotate(count);
}
