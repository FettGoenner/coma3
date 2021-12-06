#include "TileView.h"
#include <QPainter>
#include <QMouseEvent>

TileView::TileView(TileModel *tile, QColor color, QFrame *parent)
    : QFrame(parent), 
      tile(tile), 
      tileColor(color)
{
    // Cursor Style
    setCursor(Qt::PointingHandCursor);
    // Style Sheet
    setStyleSheet("QWidget{"
                  "border:1px solid red;"
                  "}");

    // update if tile changed
	connect(tile, &TileModel::changed, this, QOverload<>::of(&TileView::update)) ;
}

void TileView::mousePressEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton)
        tile->rotate() ;
}

void TileView::paintEvent(QPaintEvent * /*event*/)
{
    double width = this->width() ;
    double height = this->height() ;

    QPainter painter(this) ;

    // draw Line from middle to North/South/East/West
    // if a node happens to be there
    if (tile->north)
        painter.drawLine(width/2, height/2, width/2, height) ;
    if (tile->south)
        painter.drawLine(width/2, height/2, width/2, 0) ;
    if (tile->east)
        painter.drawLine(width/2, height/2, width, height/2) ;
    if (tile->west)
        painter.drawLine(width/2, height/2, 0, height/2) ;
}
