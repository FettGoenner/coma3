#ifndef TILECONTROLER_H
#define TILECONTROLER_H

#include <QObject>
#include <QMouseEvent>

#include "tilemodel.h"
#include "tileview.h"

class TileControler : public QObject
{
    Q_OBJECT

    TileModel*  _model;
    TileView*   _view;
public:
    explicit TileControler(TileModel*, TileView*, QObject *parent = nullptr);

    void mouseReleaseEvent(QMouseEvent*);

    bool eventFilter(QObject*, QEvent*) override;

signals:

};

#endif // TILECONTROLER_H
