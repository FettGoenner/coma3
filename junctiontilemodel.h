#ifndef JUNCTIONTILEMODEL_H
#define JUNCTIONTILEMODEL_H

#include "tilemodel.h"

class JunctionTile : public TileModel
{
public:
    JunctionTile(QObject *parent = nullptr);
    JunctionTile(QString nodes = "", QObject *parent = nullptr);
    void setNodes(QString nodes) override;
    QString getTileType() override {
        return "JunctionTile";
    }
//protected:
//    void paintEvent(QPaintEvent*) override;
};

#endif // JUNCTIONTILEMODEL_H
