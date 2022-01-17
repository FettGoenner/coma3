#ifndef JUNCTIONTILE_H
#define JUNCTIONTILE_H

#include "tilemodel.h"

class JunctionTile : public TileModel
{
public:
    explicit JunctionTile(QObject *parent = nullptr);
    explicit JunctionTile(const QVector<bool>& tile = {}, QObject *parent = nullptr);
    TileModel::TileNames type() const override{ return TileModel::JunctionTile; }
};

#endif // JUNCTIONTILE_H
