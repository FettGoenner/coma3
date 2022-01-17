#ifndef CORNERTILE_H
#define CORNERTILE_H

#include "tilemodel.h"

class CornerTile : public TileModel
{
public:
    explicit CornerTile(QObject *parent = nullptr);
    explicit CornerTile(const QVector<bool>& tile = {}, QObject *parent = nullptr);
    TileModel::TileNames type() const override {
        return TileModel::CornerTile;
    }
};

#endif // CORNERTILE_H
