#include <QPainter>

#include "TileView.h"

TileView::TileView(TileModel *tile, QColor color, QFrame *parent) :
    QFrame(parent)
  , tile(tile)
  , tileColor(color)
  , animationTimer(new QTimer)
{
    setCursor(Qt::PointingHandCursor);
    setStyleSheet("QWidget{"
                  "border:2px solid gray;"
                  "}");
    this->animationTimer->setInterval(10);
    connect(this->animationTimer, &QTimer::timeout, this, &TileView::rotateTimeout);
    connect(this, &TileView::nodeChange, this->tile, QOverload<int>::of(&TileModel::adjustNodes));
    connect(tile, &TileModel::nodesChanged, this, &TileView::adjustAngle);
    if (this->rotateAngle < tile->getAngel()) {
        this->animationAngele = tile->getAngel();
        this->animationTimer->start();
    }

}

void TileView::rotateWithAnimation(int angle)
{
    if (angle == 0)
        return;
    this->animationAngele = angle;

    this->animationTimer->start();
    emit this->nodeChange(angle/90);
}

void TileView::adjustAngle()
{
    this->rotateAngle %= 360;
    tile->rotateAngle %= 360;
    this->animationAngele = tile->rotateAngle - this->rotateAngle;
    if (this->animationAngele < 0)
        this->animationAngele += 360;
    if (this->animationAngele != 0)
        this->animationTimer->start();
}

void TileView::mouseReleaseEvent(QMouseEvent *ev)
{
    if (!this->animationTimer->isActive()){
        this->rotateWithAnimation(90);

        // send signal clicked()
        emit this->clicked();
    }
    return QWidget::mousePressEvent(ev);
}

void TileView::rotateTimeout()
{
    this->rotateAngle += 10;

    this->animationAngele -= 10;
    update();
    if (this->animationAngele == 0) {
        this->animationAngele = 90;
        this->animationTimer->stop();
    }
}

void TileView::paintEvent(QPaintEvent *ev)
{
    QString type = this->tile->getTileType();
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
    if (type == "CornerTile") {
        QLinearGradient linear(QPointF(0, height/2), QPointF(width, height/2));
        linear.setColorAt(.35, this->tileColor);
        linear.setColorAt(.5, Qt::white);
        linear.setColorAt(.65, this->tileColor);

        painter.setPen(Qt::NoPen);
        painter.setBrush(linear);


        const QPointF points[4] = {
            QPointF(2*width/5, height),
            QPointF(2*width/5, 2*height/5),
            QPointF(3*width/5, 3*height/5),
            QPointF(3*width/5, height),
        };
        painter.drawPolygon(points, 4);

        QLinearGradient linear2(QPointF(width/2, 0), QPointF(width/2, height));
        linear2.setColorAt(.35, this->tileColor);
        linear2.setColorAt(.5, Qt::white);
        linear2.setColorAt(.65, this->tileColor);

        painter.setBrush(linear2);

        const QPointF points2[4] = {
            QPointF(2*width/5, 2*height/5),
            QPointF(3*width/5, 3*height/5),
            QPointF(width, 3*height/5),
            QPointF(width, 2*height/5),
        };

        painter.drawPolygon(points2, 4);
    } else if (type == "EndTile") {
        QLinearGradient linear(QPointF(0, height/2), QPointF(width, height/2));
        linear.setColorAt(.35, this->tileColor);
        linear.setColorAt(.5, Qt::white);
        linear.setColorAt(.65, this->tileColor);
        painter.setPen(Qt::NoPen);
        painter.setBrush(linear);

        const QPointF points[4] = {
            QPointF(2*width/5, 0),
            QPointF(3*width/5, 0),
            QPointF(3*width/5, height/2),
            QPointF(2*width/5, height/2),
        };
        painter.drawPolygon(points, 4);
    } else if (type == "JunctionTile") {
        QLinearGradient linear(QPointF(width/2, 0), QPointF(width/2, height));
        linear.setColorAt(.35, this->tileColor);
        linear.setColorAt(.5, Qt::white);
        linear.setColorAt(.65, this->tileColor);

        painter.setPen(Qt::NoPen);
        painter.setBrush(linear);

        const QPointF points[4] = {
            QPointF(0, 2*height/5),
            QPointF(0, 3*height/5),
            QPointF(width, 3*height/5),
            QPointF(width, 2*height/5),
        };
        painter.drawPolygon(points, 4);

        QLinearGradient linear2(QPointF(0, height/2), QPointF(width, height/2));
        linear2.setColorAt(.35, this->tileColor);
        linear2.setColorAt(.5, Qt::white);
        linear2.setColorAt(.65, this->tileColor);

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
    } else if (type == "LineTile") {
        QLinearGradient linear(QPointF(0, height/2), QPointF(width, height/2));
        linear.setColorAt(.35, this->tileColor);
        linear.setColorAt(.5, Qt::white);
        linear.setColorAt(.65, this->tileColor);
        painter.setPen(Qt::NoPen);
        painter.setBrush(linear);

        const QPointF points[4] = {
            QPointF(2*width/5, 0),
            QPointF(3*width/5, 0),
            QPointF(3*width/5, height),
            QPointF(2*width/5, height),
        };
        painter.drawPolygon(points, 4);
    }

    return QWidget::paintEvent(ev);
}

