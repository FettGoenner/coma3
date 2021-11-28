#ifndef JUNCTIONTILE_H
#define JUNCTIONTILE_H

#include "tile.h"

class JunctionTile : public Tile
{
public:
    JunctionTile(QColor color = Qt::blue);
    JunctionTile(QString nodes = "",QColor color = Qt::blue);
    void setNodes(QString nodes) override;
protected:
    void paintEvent(QPaintEvent*) override;
};

#endif // JUNCTIONTILE_H
