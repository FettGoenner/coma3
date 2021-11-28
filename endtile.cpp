#include "endtile.h"
#include <QPainter>
#include <QPen>

EndTile::EndTile(QColor color)
{
    this->nodes[0] = true;
    nodeColor = color;

}

EndTile::EndTile(QString nodes, QColor color)
{
    this->setNodes(nodes);
    this->nodeColor = color;
}

void EndTile::setNodes(QString nodes)
{
    this->clearNodes();
    if (nodes.isEmpty()) {
        this->nodes[0] = true;
    } else if (nodes.size() == 1) {
        this->nodes[nodes.toInt()] = true;
    } else {
        throw "The nodes does not match to any EndTiles";
    }
    this->rotateAngle = nodes.toInt()*90;
    update();
}

void EndTile::paintEvent(QPaintEvent *ev)
{
    int width = this->width(), height = this->height();
    // draw the deflaut headNode, top
    QPainter painter(this);

    painter.translate(width/2, height/2);
    painter.rotate(rotateAngle);
    painter.translate(-width/2, -height/2);
    QPen pen;
    pen.setWidth(0);
    pen.setColor(QColor(0, 0, 0, 0));

    QLinearGradient linear(QPointF(0, height/2), QPointF(width, height/2));
    linear.setColorAt(.35, nodeColor);
    linear.setColorAt(.5, Qt::white);
    linear.setColorAt(.65, nodeColor);
    painter.setPen(pen);
    painter.setBrush(linear);

    const QPointF points[4] = {
        QPointF(2*width/5, 0),
        QPointF(3*width/5, 0),
        QPointF(3*width/5, height/2),
        QPointF(2*width/5, height/2),
    };
    painter.drawPolygon(points, 4);

    return QWidget::paintEvent(ev);
}

