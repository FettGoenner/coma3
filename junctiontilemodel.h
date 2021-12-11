#ifndef JUNCTIONTILEMODEL_H
#define JUNCTIONTILEMODEL_H

#include "tilemodel.h"

class JunctionTile : public TileModel
{
public:
    JunctionTile(QObject *parent = nullptr);
    JunctionTile(const QVector<bool>& tile = {}, QObject *parent = nullptr);
//    void setNodes(bool north, bool east, bool south, bool west) override{};
    void setNodes(const QVector<bool>& tile) override;
//    bool isValidTile() override;
    QString getTileType() override {
        return "JunctionTile";
    }
//protected:
//    void paintEvent(QPaintEvent*) override;
};

#endif // JUNCTIONTILEMODEL_H
