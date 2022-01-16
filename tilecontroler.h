#ifndef TILECONTROLER_H
#define TILECONTROLER_H

#include <QObject>

class QMouseEvent;

class TileModel;
class TileView;

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
