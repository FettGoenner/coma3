#include "node.h"
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>

Node::Node(QFrame *parent) :
    QFrame(parent)
{
    setCursor(Qt::PointingHandCursor);
    setStyleSheet("QWidget{"
                  "border:2px solid gray;"
                  "}");
}

void Node::rotate90()
{
    bool last = orientation[3];
    orientation.pop_back();
    orientation.insert(orientation.begin(), last);
    rotateAngle == 270 ? rotateAngle = 0 : rotateAngle += 90;
    update();
}

void Node::mouseReleaseEvent(QMouseEvent *ev)
{
    rotate90();
    // send signal clicked()
    emit clicked();
    return QWidget::mousePressEvent(ev);
}
