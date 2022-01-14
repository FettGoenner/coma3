#ifndef LINETILEMODEL_H
#define LINETILEMODEL_H

#include "tilemodel.h"

class LineTile : public TileModel
{
public:
    LineTile(QObject *parent = nullptr);
    LineTile(const QVector<bool>& tile = {}, QObject *parent = nullptr);
//    void setNodes(bool north, bool east, bool south, bool west) override{};
    void setNodes(const QVector<bool>& tile) override;
    int getTileType() const override {
        return TileModel::LineTile;
    }
};

#endif // LINETILEMODEL_H
