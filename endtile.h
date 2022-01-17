#ifndef ENDTILE_H
#define ENDTILE_H

#include "tilemodel.h"

class EndTile : public TileModel
{
public:
    explicit EndTile(QObject *parent = nullptr);
    explicit EndTile(const QVector<bool>& tile = {}, QObject *parent = nullptr);
    TileModel::TileNames type() const override { return TileModel::EndTile; }
};

#endif // ENDTILE_H
