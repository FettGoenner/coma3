#ifndef ENDTILEMODEL_H
#define ENDTILEMODEL_H

#include "tilemodel.h"

class EndTile : public TileModel
{
public:
    EndTile(QObject *parent = nullptr);
    EndTile(const QVector<bool>& tile = {}, QObject *parent = nullptr);
//    void setNodes(bool north, bool east, bool south, bool west) override{};
    void setNodes(const QVector<bool>& tile) override;
    int getTileType() override {
        return TileModel::EndTile;
    }
};

#endif // ENDTILEMODEL_H
