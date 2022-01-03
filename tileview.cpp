#include <QPainter>
#include <QtMath>

#include "tileview.h"

TileView::TileView(TileModel *tile, QColor color, QFrame *parent) :
    QFrame(parent)
  , tile(tile)
  , tileColor(color)
  , animationTimer(new QTimer)
//  , hintAnimationTimer(new QTimer)
{
    setCursor(Qt::PointingHandCursor);
    this->setBackgroundColor(Qt::transparent);

    this->animationTimer->setInterval(7);
//    this->hintAnimationTimer->setInterval(60);

    //
    connect(this->animationTimer, &QTimer::timeout, this, &TileView::rotateTimeout);
//    connect(this->hintAnimationTimer, &QTimer::timeout, this, &TileView::hintAnimationTimeout);

    connect(tile, &TileModel::resetedTile, this, [=]() {
        this->setBackgroundColor(Qt::transparent);
//        this->stopHintAnimation();
    });
    connect(this, &TileView::nodeChange, this->tile, &TileModel::adjustNodes);

    // set background color to yellow if the tile rotated by hint
    connect(this->tile, &TileModel::rotatedByHint, this, [=]() {
        this->setBackgroundColor(Qt::yellow);
    });
    connect(this->tile, &TileModel::nodesChanged, this, &TileView::adjustAngle);
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

//void TileView::startHintAnimation()
//{
//    this->hintAnimationTimer->start();
//}

//void TileView::stopHintAnimation()
//{
//    this->hintAnimationTimer->stop();
//    this->alphaValueF = 0;
//    update();
//}

void TileView::isConnected(bool connected)
{
    if (connected){
        this->setTileColor(QColor(0, 200, 0)); // change tile color to green
        this->canBeClicked = false;
    }
    else {
        this->setTileColor(Qt::blue);
        this->canBeClicked = true;
    }
}

//void TileView::hintAnimationTimeout()
//{

//    this->alphaValueF += 0.3;
//    update();
//}

void TileView::adjustAngle()
{
    this->rotateAngle %= 360;
    int tileRotateAngel = tile->rotateAngle % 360;
    this->animationAngele = tileRotateAngel - this->rotateAngle;
    if (this->animationAngele < 0)
        this->animationAngele += 360;
    if (this->animationAngele != 0)
        this->animationTimer->start();
}

void TileView::setBackgroundColor(QColor color)
{
    if (color == Qt::transparent) {
        this->canBeClicked = true;
        this->setStyleSheet("QWidget{"
                            "border:2px solid gray;"
                            "}");
    }
    else if (color == Qt::yellow) {
        this->canBeClicked = false;
        this->setStyleSheet("QWidget{"
                            "border:2px solid gray;"
                            "background-color: rgb(255, 255, 0);"
                            "}");
    }
}

void TileView::setTileColor(QColor color)
{
    this->tileColor = color;
    this->update();
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
    if (!this->animationTimer->isActive() && this->canBeClicked/* && !this->hintAnimationTimer->isActive()*/) {
        this->rotateWithAnimation(90);

        // send signal clicked()
        emit this->clicked();
    } /*else if (this->hintAnimationTimer->isActive()) {
        this->setBackgroundColor(Qt::yellow);
        this->canBeClicked = false;
        emit this->clickedWhileHint(this->tile);
    }*/
    return QWidget::mousePressEvent(ev);
}

void TileView::paintEvent(QPaintEvent *ev)
{
    int type = this->tile->getTileType();
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
//    this->tileColor.setAlphaF((qCos(this->alphaValueF) + 1.5)/2.5);
    painter.setPen(Qt::NoPen);
    if (type == TileModel::CornerTile) {
        QLinearGradient linear(QPointF(0, height/2), QPointF(width, height/2));
        linear.setColorAt(.35, this->tileColor);
        linear.setColorAt(.5, Qt::white);
        linear.setColorAt(.65, this->tileColor);


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
    } else if (type == TileModel::EndTile) {
        QLinearGradient linear(QPointF(0, height/2), QPointF(width, height/2));
        linear.setColorAt(.35, this->tileColor);
        linear.setColorAt(.5, Qt::white);
        linear.setColorAt(.65, this->tileColor);
        painter.setBrush(linear);

        const QPointF points[4] = {
            QPointF(2*width/5, 0),
            QPointF(3*width/5, 0),
            QPointF(3*width/5, height/2),
            QPointF(2*width/5, height/2),
        };
        painter.drawPolygon(points, 4);

    } else if (type == TileModel::JunctionTile) {
        QLinearGradient linear(QPointF(width/2, 0), QPointF(width/2, height));
        linear.setColorAt(.35, this->tileColor);
        linear.setColorAt(.5, Qt::white);
        linear.setColorAt(.65, this->tileColor);

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

    } else if (type == TileModel::LineTile) {
        QLinearGradient linear(QPointF(0, height/2), QPointF(width, height/2));
        linear.setColorAt(.35, this->tileColor);
        linear.setColorAt(.5, Qt::white);
        linear.setColorAt(.65, this->tileColor);

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

