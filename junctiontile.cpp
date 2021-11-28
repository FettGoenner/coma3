#include "junctiontile.h"
#include <QPainter>
#include <QPen>

JunctionTile::JunctionTile(QColor color)
{
    this->nodes[1] = true;
    this->nodes[2] = true;
    this->nodes[3] = true;
    this->nodeColor = color;
}

JunctionTile::JunctionTile(QString nodes, QColor color)
{
    this->setNodes(nodes);
    this->nodeColor = color;
}

void JunctionTile::setNodes(QString nodes)
{
    this->clearNodes();
    if (nodes.isEmpty()) {
        this->nodes[1] = true;
        this->nodes[2] = true;
        this->nodes[3] = true;
    } else if (nodes.size() == 3) {
        for (const QChar &s : nodes)
            this->nodes[s.digitValue()] = true;
    } else {
        throw "The nodes does not match to any JunctionTiles";
    }
    for (int i = 0; i < 4; ++i) {
        if (!this->nodes[i]) {
            this->rotateAngle = i*90;
            break;
        }
    }
    update();
}

void JunctionTile::paintEvent(QPaintEvent *ev)
{
    int width = this->width(), height = this->height();
    // draw the deflaut TurnNode, from right to bottom
    QPainter painter(this);
    painter.translate(width/2, height/2);
    painter.rotate(this->rotateAngle);
    painter.translate(-width/2, -height/2);

    QPen pen;
    pen.setWidth(3);
//    painter.drawRect(0, 0, width, height);

    pen.setWidth(0);
    pen.setColor(QColor(0, 0, 0, 0));

    QLinearGradient linear(QPointF(width/2, 0), QPointF(width/2, height));
    linear.setColorAt(.35, nodeColor);
    linear.setColorAt(.5, Qt::white);
    linear.setColorAt(.65, nodeColor);

    painter.setPen(pen);
    painter.setBrush(linear);

    const QPointF points[4] = {
        QPointF(0, 2*height/5),
        QPointF(0, 3*height/5),
        QPointF(width, 3*height/5),
        QPointF(width, 2*height/5),
    };
    painter.drawPolygon(points, 4);

    QLinearGradient linear2(QPointF(0, height/2), QPointF(width, height/2));
    linear2.setColorAt(.35, nodeColor);
    linear2.setColorAt(.5, Qt::white);
    linear2.setColorAt(.65, nodeColor);

    painter.setBrush(linear2);

    const QPointF points2[4] = {
        QPointF(2*width/5, 3*height/5),
        QPointF(width/2, height/2),
        QPointF(width/2, height),
        QPointF(2*width/5, height),
    };
    painter.drawPolygon(points2, 4);

    const QPointF points3[4] = {
        QPointF(3*width/5, 3*height/5),
        QPointF(width/2, height/2),
        QPointF(width/2, height),
        QPointF(3*width/5, height),
    };
    painter.drawPolygon(points3, 4);

    return QWidget::paintEvent(ev);
}
