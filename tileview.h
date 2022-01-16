#ifndef TILEVIEW_H
#define TILEVIEW_H

#include <QFrame>

class TileModel;

class TileView : public QFrame
{
    Q_OBJECT
    TileModel* _model;
public:
    explicit TileView( TileModel*, QColor, QFrame* parent = nullptr );

protected:
    void paintEvent( QPaintEvent* ) override;
};

#endif // TILEVIEW_H
