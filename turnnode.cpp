#include "turnnode.h"
#include <QPainter>
#include <QPen>
#include <QTimer>

TurnNode::TurnNode(QColor color)
{
    this->orientation[1] = true;
    this->orientation[2] = true;
    nodeColor = color;
}

void TurnNode::paintEvent(QPaintEvent *ev)
{
    int width = this->width(), height = this->height();
    // draw the deflaut TurnNode, from right to bottom
    QPainter painter(this);
    painter.translate(width/2, height/2);
    painter.rotate(rotateAngle);
    painter.translate(-width/2, -height/2);

    QPen pen;
    pen.setWidth(3);
    painter.drawRect(0, 0, width, height);

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
