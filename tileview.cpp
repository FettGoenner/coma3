#include <QPainter>
#include <QtMath>

#include "tileview.h"
#include "tilemodel.h"

TileView::TileView(TileModel* model, QColor color, QFrame* parent)
    : QFrame(parent),
      _model(model)
{
    _model->setColor(color);
    setCursor(Qt::PointingHandCursor);


    setCursor(Qt::PointingHandCursor);
    _model->setBgdColor(Qt::transparent);
    setStyleSheet(_model->bgdColor());

    connect(_model, &TileModel::tileChanged, this, QOverload<>::of(&TileView::update));
    connect(_model, &TileModel::bgcChanged, this, [=]() {
        setStyleSheet(_model->bgdColor());
    });
}

// draws the Tile visual
void TileView::paintEvent(QPaintEvent *ev)
{
    auto type = _model->type();
    double w = width(), h = height();
    // draw the deflaut TurnNode, from right to bottom
    QPainter painter(this);
    painter.translate(w/2, h/2);
    if (_model->angle() % 180 != 0) {
        double temp = w;
        w = h;
        h = temp;
    }
    painter.rotate(_model->angle());
    painter.translate(-w/2, -h/2);
    //    tileColor.setAlphaF((qCos(alphaValueF) + 1.5)/2.5);
    painter.setPen(Qt::NoPen);
    if (type == TileModel::CornerTile) {
        QLinearGradient linear(QPointF(0, h/2), QPointF(w, h/2));
        linear.setColorAt(.35, _model->color());
        linear.setColorAt(.5, Qt::white);
        linear.setColorAt(.65, _model->color());


        painter.setBrush(linear);

        const QPointF points[4] = {
            QPointF(2*w/5, h),
            QPointF(2*w/5, 2*h/5),
            QPointF(3*w/5, 3*h/5),
            QPointF(3*w/5, h),
        };
        painter.drawPolygon(points, 4);

        QLinearGradient linear2(QPointF(w/2, 0), QPointF(w/2, h));
        linear2.setColorAt(.35, _model->color());
        linear2.setColorAt(.5, Qt::white);
        linear2.setColorAt(.65, _model->color());

        painter.setBrush(linear2);

        const QPointF points2[4] = {
            QPointF(2*w/5, 2*h/5),
            QPointF(3*w/5, 3*h/5),
            QPointF(w, 3*h/5),
            QPointF(w, 2*h/5),
        };

        painter.drawPolygon(points2, 4);
    } else if (type == TileModel::EndTile) {
        QLinearGradient linear(QPointF(0, h/2), QPointF(w, h/2));
        linear.setColorAt(.35, _model->color());
        linear.setColorAt(.5, Qt::white);
        linear.setColorAt(.65, _model->color());
        painter.setBrush(linear);

        const QPointF points[4] = {
            QPointF(2*w/5, 0),
            QPointF(3*w/5, 0),
            QPointF(3*w/5, h/2),
            QPointF(2*w/5, h/2),
        };
        painter.drawPolygon(points, 4);

    } else if (type == TileModel::JunctionTile) {
        QLinearGradient linear(QPointF(w/2, 0), QPointF(w/2, h));
        linear.setColorAt(.35, _model->color());
        linear.setColorAt(.5, Qt::white);
        linear.setColorAt(.65, _model->color());

        painter.setBrush(linear);

        const QPointF points[4] = {
            QPointF(0, 2*h/5),
            QPointF(0, 3*h/5),
            QPointF(w, 3*h/5),
            QPointF(w, 2*h/5),
        };
        painter.drawPolygon(points, 4);

        QLinearGradient linear2(QPointF(0, h/2), QPointF(w, h/2));
        linear2.setColorAt(.35, _model->color());
        linear2.setColorAt(.5, Qt::white);
        linear2.setColorAt(.65, _model->color());

        painter.setBrush(linear2);

        const QPointF points2[4] = {
            QPointF(2*w/5, 3*h/5),
            QPointF(w/2, h/2),
            QPointF(w/2, h),
            QPointF(2*w/5, h),
        };
        painter.drawPolygon(points2, 4);

        const QPointF points3[4] = {
            QPointF(3*w/5, 3*h/5),
            QPointF(w/2, h/2),
            QPointF(w/2, h),
            QPointF(3*w/5, h),
        };
        painter.drawPolygon(points3, 4);

    } else if (type == TileModel::LineTile) {
        QLinearGradient linear(QPointF(0, h/2), QPointF(w, h/2));
        linear.setColorAt(.35, _model->color());
        linear.setColorAt(.5, Qt::white);
        linear.setColorAt(.65, _model->color());

        painter.setBrush(linear);

        const QPointF points[4] = {
            QPointF(2*w/5, 0),
            QPointF(3*w/5, 0),
            QPointF(3*w/5, h),
            QPointF(2*w/5, h),
        };
        painter.drawPolygon(points, 4);
    }

    return QWidget::paintEvent(ev);
}
