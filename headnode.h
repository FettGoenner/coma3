#ifndef HEADNODE_H
#define HEADNODE_H

#include "node.h"

class HeadNode : public Node
{
public:
    HeadNode(QColor color = Qt::blue);
protected:
    void paintEvent(QPaintEvent*) override;
};

#endif // HEADNODE_H
