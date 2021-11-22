#ifndef TURNNODE_H
#define TURNNODE_H

#include "node.h"

class TurnNode : public Node
{
public:
    TurnNode(QColor color = Qt::blue);
protected:
    void paintEvent(QPaintEvent*) override;
};

#endif // TURNNODE_H
