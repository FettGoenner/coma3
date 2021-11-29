#include "cornertile.h"
#include <QPainter>
#include <QPen>
#include <QTimer>
#include <QDebug>

CornerTile::CornerTile(QColor color)
{
    this->nodes[1] = true;
    this->nodes[2] = true;
    this->nodeColor = color;
}

CornerTile::CornerTile(QString nodes, QColor color)
{
    this->setNodes(nodes);
    this->nodeColor = color;
}

void CornerTile::setNodes(QString nodes)
{
    this->clearNodes();
    if (nodes.isEmpty()) {
        this->nodes[1] = true;
        this->nodes[2] = true;
    } else if (nodes.size() == 2) {
        int first = nodes[0].digitValue(), second = nodes[1].digitValue();
        if (first > second) {
            int temp = first;
            first = second;
            second = temp;
        }
        if (second - first == 1 || second - first == 3) {
            this->nodes[first] = true;
            this->nodes[second] = true;
        } else
            throw "The nodes does not match to any CornerTiles";

    } else {
        throw "The nodes does not match to any CornerTiles";
    }
    int index1 = 1, index2 = 2;
    this->rotateAngle = 0;
    for (int i = 0; i < 4; ++i) {
        if (this->nodes[index1] && this->nodes[index2])
            break;
        ++index1, ++index2;
        this->rotateAngle += 90;
        if (index1 == 4)
            index1 = 0;
        if (index2 == 4)
            index2 = 0;
    }
    update();
}

void CornerTile::paintEvent(QPaintEvent *ev)
{
    double width = this->width(), height = this->height();
    // draw the deflaut TurnNode, from right to bottom
    QPainter painter(this);
    painter.translate(width/2, height/2);
    if (this->rotateAngle % 180 != 0) {
        double temp = width;
        width = height;
        height = temp;
    }
    painter.rotate(this->rotateAngle);
    painter.translate(-width/2, -height/2);

    QPen pen;
    pen.setWidth(3);
    //    painter.drawRect(0, 0, width, height);

    pen.setWidth(0);
    pen.setColor(QColor(0, 0, 0, 0));

    QLinearGradient linear(QPointF(0, height/2), QPointF(width, height/2));
    linear.setColorAt(.35, nodeColor);
    linear.setColorAt(.5, Qt::white);
    linear.setColorAt(.65, nodeColor);

    painter.setPen(pen);
    painter.setBrush(linear);


    const QPointF points[4] = {
        QPointF(2*width/5, height),
        QPointF(2*width/5, 2*height/5),
        QPointF(3*width/5, 3*height/5),
        QPointF(3*width/5, height),
    };
    painter.drawPolygon(points, 4);

    QLinearGradient linear2(QPointF(width/2, 0), QPointF(width/2, height));
    linear2.setColorAt(.35, nodeColor);
    linear2.setColorAt(.5, Qt::white);
    linear2.setColorAt(.65, nodeColor);

    painter.setBrush(linear2);

    const QPointF points2[4] = {
        QPointF(2*width/5, 2*height/5),
        QPointF(3*width/5, 3*height/5),
        QPointF(width, 3*height/5),
        QPointF(width, 2*height/5),
    };

    painter.drawPolygon(points2, 4);

    return QWidget::paintEvent(ev);
}
