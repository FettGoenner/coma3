#include "straightnode.h"
#include <QPainter>
#include <QPen>

StraightNode::StraightNode(QColor color)
{
    this->orientation[0] = true;
    this->orientation[2] = true;
    nodeColor = color;
}

void StraightNode::paintEvent(QPaintEvent *ev)
{
    int width = this->width(), height = this->height();
    // draw the deflaut StraightNode, from top to bottom
    QPainter painter(this);

    painter.translate(width/2, height/2);
    painter.rotate(rotateAngle);
    painter.translate(-width/2, -height/2);

    QPen pen;
    pen.setWidth(3);
    painter.drawRect(0, 0, width, height);

    pen.setWidth(1);
    pen.setColor(QColor(0, 0, 0, 0));

    QLinearGradient linear(QPointF(0, height/2), QPointF(width, height/2));
    linear.setColorAt(.35, nodeColor);
    linear.setColorAt(.5, Qt::white);
    linear.setColorAt(.65, nodeColor);
    painter.setPen(pen);
    painter.setBrush(linear);


    painter.drawRect(2*width/5, -1, width/5, height+1);

    return QWidget::paintEvent(ev);
}
