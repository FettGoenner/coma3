#include <QPainter>
#include <QtMath>

#include "tileview.h"
#include "tilemodel.h"

TileView::TileView( TileModel* model, QColor color, QFrame* parent )
    : QFrame( parent ),
      _model( model )
{
    _model->setColor( color );
    setCursor( Qt::PointingHandCursor );


    setCursor(Qt::PointingHandCursor);
    _model->setBgdColor(Qt::transparent);
    this->setStyleSheet(_model->bgdColor());  

    connect(_model, &TileModel::tileChanged, this, QOverload<>::of(&TileView::update));
    connect(_model, &TileModel::bgcChanged, this, [=]() {
        setStyleSheet(_model->bgdColor());
    });
}

// draws the Tile visual
void TileView::paintEvent( QPaintEvent *ev)
{
    auto type = _model->type();
    double width = this->width(), height = this->height();
    // draw the deflaut TurnNode, from right to bottom
    QPainter painter(this);
    painter.translate(width/2, height/2);
    if (_model->angle() % 180 != 0) {
        double temp = width;
        width = height;
        height = temp;
    }
    painter.rotate(_model->angle());
    painter.translate(-width/2, -height/2);
    //    this->tileColor.setAlphaF((qCos(this->alphaValueF) + 1.5)/2.5);
    painter.setPen(Qt::NoPen);
    if (type == TileModel::CornerTile) {
        QLinearGradient linear(QPointF(0, height/2), QPointF(width, height/2));
        linear.setColorAt(.35, _model->color());
        linear.setColorAt(.5, Qt::white);
        linear.setColorAt(.65, _model->color());


        painter.setBrush(linear);

        const QPointF points[4] = {
            QPointF(2*width/5, height),
            QPointF(2*width/5, 2*height/5),
            QPointF(3*width/5, 3*height/5),
            QPointF(3*width/5, height),
        };
        painter.drawPolygon(points, 4);

        QLinearGradient linear2(QPointF(width/2, 0), QPointF(width/2, height));
        linear2.setColorAt(.35, _model->color());
        linear2.setColorAt(.5, Qt::white);
        linear2.setColorAt(.65, _model->color());

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
        linear.setColorAt(.35, _model->color());
        linear.setColorAt(.5, Qt::white);
        linear.setColorAt(.65, _model->color());
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
        linear.setColorAt(.35, _model->color());
        linear.setColorAt(.5, Qt::white);
        linear.setColorAt(.65, _model->color());

        painter.setBrush(linear);

        const QPointF points[4] = {
            QPointF(0, 2*height/5),
            QPointF(0, 3*height/5),
            QPointF(width, 3*height/5),
            QPointF(width, 2*height/5),
        };
        painter.drawPolygon(points, 4);

        QLinearGradient linear2(QPointF(0, height/2), QPointF(width, height/2));
        linear2.setColorAt(.35, _model->color());
        linear2.setColorAt(.5, Qt::white);
        linear2.setColorAt(.65, _model->color());

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
        linear.setColorAt(.35, _model->color());
        linear.setColorAt(.5, Qt::white);
        linear.setColorAt(.65, _model->color());

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
