#ifndef CORNERTILEMODEL_H
#define CORNERTILEMODEL_H

#include "tilemodel.h"

class CornerTile : public TileModel
{
public:
    CornerTile(QObject *parent = nullptr);
    CornerTile(const QVector<bool>& tile = {}, QObject *parent = nullptr);
//    void setNodes(bool north, bool east, bool south, bool west) override {};
    void setNodes(const QVector<bool>& tile) override;
    bool isValidTile() override;
    QString getTileType() override {
        return "CornerTile";
    }

//protected:
//    void paintEvent(QPaintEvent*) override;
};

#endif // CORNERTILEMODEL_H
