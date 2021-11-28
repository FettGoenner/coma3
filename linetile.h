#ifndef LINETILE_H
#define LINETILE_H

#include "tile.h"

class LineTile : public Tile
{
public:
    LineTile(QColor color = Qt::blue);
    LineTile(QString nodes = "",QColor color = Qt::blue);
    void setNodes(QString nodes) override;
protected:
    void paintEvent(QPaintEvent*) override;
};

#endif // LINETILE_H
