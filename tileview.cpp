#include <QPainter>
#include <QtMath>

#include "tileview.h"

TileView::TileView(TileModel *tile, QColor color, QFrame *parent) :
    QFrame(parent)
  , tile(tile)
  , tileColor(color)
  , animationTimer(new QTimer)
  , hintAnimationTimer(new QTimer)
{
    setCursor(Qt::PointingHandCursor);
    this->setColor(Qt::transparent);
    this->animationTimer->setInterval(10);
    this->hintAnimationTimer->setInterval(60);
    connect(this->animationTimer, &QTimer::timeout, this, &TileView::rotateTimeout);
    connect(this->hintAnimationTimer, &QTimer::timeout, this, &TileView::hintAnimationTimeout);

    connect(tile, &TileModel::resetTile, this, [=]() {
        this->setColor(Qt::transparent);
    });
    connect(this, &TileView::nodeChange, this->tile, &TileModel::adjustNodes);
    connect(tile, &TileModel::nodesChanged, this, &TileView::adjustAngle);
    if (this->rotateAngle < tile->getAngle()) {
        this->animationAngele = tile->getAngle();
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

void TileView::startHintAnimation()
{
    this->hintAnimationTimer->start();
}

void TileView::stopHintAnimation()
{
    this->hintAnimationTimer->stop();
    this->alphaValueF = 0;
    update();
}

void TileView::hintAnimationTimeout()
{

    this->alphaValueF += 0.5;
    update();
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

void TileView::setColor(QColor color)
{
    if (color == Qt::transparent)
        this->setStyleSheet("QWidget{"
                            "border:2px solid gray;"
                            "}");
    else if (color == Qt::green)
        this->setStyleSheet("QWidget{"
                            "border:2px solid gray;"
                            "background-color: rgb(0, 255, 0);"
                            "}");
    else if (color == Qt::yellow)
        this->setStyleSheet("QWidget{"
                            "border:2px solid gray;"
                            "background-color: rgb(255, 255, 0);"
                            "}");
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

void TileView::mouseReleaseEvent(QMouseEvent *ev)
{
    if (!this->animationTimer->isActive() && this->canBeClicked && !this->hintAnimationTimer->isActive()) {
        this->rotateWithAnimation(90);

        // send signal clicked()
        emit this->clicked();
    } else if (this->hintAnimationTimer->isActive()) {
        this->setColor(Qt::yellow);
        this->canBeClicked = false;
        emit this->clickedWhileHint(this->tile);
    }
    return QWidget::mousePressEvent(ev);
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
    this->tileColor.setAlphaF((qCos(this->alphaValueF) + 1.5)/2.5);

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

