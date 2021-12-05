#ifndef LINETILEMODEL_H
#define LINETILEMODEL_H

#include "tilemodel.h"

class LineTile : public TileModel
{
public:
    LineTile(QObject *parent = nullptr);
    LineTile(QString nodes = "", QObject *parent = nullptr);
    void setNodes(QString nodes) override;
    QString getTileType() override {
        return "LineTile";
    }
//protected:
//    void paintEvent(QPaintEvent*) override;
};

#endif // LINETILEMODEL_H
