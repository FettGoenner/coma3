#ifndef CORNERTILEMODEL_H
#define CORNERTILEMODEL_H

#include "tilemodel.h"

class CornerTile : public TileModel
{
public:
    CornerTile(QObject *parent = nullptr);
    CornerTile(QString nodes = "", QObject *parent = nullptr);
    void setNodes(QString nodes) override;
    QString getTileType() override {
        return "CornerTile";
    }
//protected:
//    void paintEvent(QPaintEvent*) override;
};

#endif // CORNERTILEMODEL_H
