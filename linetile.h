#ifndef LINETILE_H
#define LINETILE_H

#include "tilemodel.h"

class LineTile : public TileModel
{
public:
    explicit LineTile(QObject *parent = nullptr);
    explicit LineTile(const QVector<bool>& tile = {}, QObject *parent = nullptr);
    TileModel::TileNames type() const override{ return TileModel::LineTile; }
};

#endif // LINETILE_H
