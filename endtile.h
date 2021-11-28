#ifndef ENDTILE_H
#define ENDTILE_H

#include "tile.h"

class EndTile : public Tile
{
public:
    EndTile(QColor color = Qt::blue);
    EndTile(QString nodes = "",QColor color = Qt::blue);
    void setNodes(QString nodes) override;
protected:
    void paintEvent(QPaintEvent*) override;
};

#endif // ENDTILE_H
