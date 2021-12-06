#ifndef TILEVIEW_H
#define TILEVIEW_H

#include <QFrame>
#include "TileModel.h"

class TileView
    : public QFrame
{
    Q_OBJECT

/*** private attributes ***/

    // das TileModel
    TileModel *tile ;
    // die Farbe
    QColor tileColor ;

public:
    TileView(TileModel *tile, QColor color, QFrame *parent = nullptr) ;
    void mousePressEvent(QMouseEvent * event) override;

protected:
    void paintEvent(QPaintEvent*) override;
} ;

#endif // TILEVIEW_H
