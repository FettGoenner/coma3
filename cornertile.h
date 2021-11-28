#ifndef CORNERTILE_H
#define CORNERTILE_H

#include "tile.h"

class CornerTile : public Tile
{
public:
    CornerTile(QColor color = Qt::blue);
    CornerTile(QString nodes = "",QColor color = Qt::blue);
    void setNodes(QString nodes) override;
protected:
    void paintEvent(QPaintEvent*) override;
};

#endif // CORNERTILE_H
