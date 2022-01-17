#include "linetile.h"

LineTile::LineTile(QObject *parent) : TileModel(parent)
{ setNodes({1,0,1,0}); }

LineTile::LineTile(const QVector<bool>& vec, QObject *parent)
    :TileModel({1,0,1,0},parent)
{
    if (TileModel::typeByVector(vec) != TileModel::LineTile)
        throw "LineTile is invalid!";

    if (!vec[0])
        if (angle() % 360 != 270)
           rotate(1);

}
