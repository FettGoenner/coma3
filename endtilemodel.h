#ifndef ENDTILEMODEL_H
#define ENDTILEMODEL_H

#include "tilemodel.h"

class EndTile : public TileModel
{
public:
    EndTile(QObject *parent = nullptr);
    EndTile(QString nodes = "", QObject *parent = nullptr);
    void setNodes(QString nodes) override;
    QString getTileType() override {
        return "EndTile";
    }
//protected:
//    void paintEvent(QPaintEvent*) override;
};

#endif // ENDTILEMODEL_H
