#include "endtile.h"

EndTile::EndTile(QObject *parent) : TileModel(parent)
{ setNodes({1,0,0,0}); }

EndTile::EndTile(const QVector<bool>& vec, QObject *parent)
    :TileModel({1,0,0,0},parent)
{
    if (TileModel::typeByVector(vec) != TileModel::EndTile)
        throw "EndTile is invalid!";

    for (int count = 0; count < 4; ++count)
        if (vec[count])
            rotate(count);
}
