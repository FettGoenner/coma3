#ifndef STRAIGHTNODE_H
#define STRAIGHTNODE_H

#include "node.h"

class StraightNode : public Node
{
public:
    StraightNode(QColor color = Qt::blue);
protected:
    void paintEvent(QPaintEvent*) override;
};

#endif // STRAIGHTNODE_H
