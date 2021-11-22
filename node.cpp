#include "node.h"
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>

Node::Node(QWidget *parent) :
    QWidget(parent)
{

}

void Node::rotate90()
{
    bool last = orientation[3];
    orientation.pop_back();
    orientation.insert(orientation.begin(), last);
}

void Node::mouseReleaseEvent(QMouseEvent *ev)
{
    rotate90();
    rotateAngle == 270 ? rotateAngle = 0 : rotateAngle += 90;
    update();

//    for (int &i : this->orientation)
//        qDebug() << i;
    emit clicked();
    return QWidget::mousePressEvent(ev);
}
