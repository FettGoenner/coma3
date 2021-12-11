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
    bool isValidTile() override;
    QString getTileType() override {
        return "EndTile";
    }
//protected:
//    void paintEvent(QPaintEvent*) override;
};

#endif // ENDTILEMODEL_H
