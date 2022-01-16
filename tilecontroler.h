#ifndef TILECONTROLER_H
#define TILECONTROLER_H

#include <QObject>
#include <QEvent>
#include <QMouseEvent>
#include <QWidget>

#include "tilemodel.h"
#include "tileview.h"

class TileControler : public QObject
{
    Q_OBJECT
private:
    TileModel *m_model;
    TileView *m_view;
public:
    explicit TileControler(TileModel *model, TileView *view, QObject *parent = nullptr);

protected:
    void mouseReleaseEvent(QMouseEvent *ev);

    virtual bool eventFilter(QObject *watched, QEvent *event) override;
signals:
    void clicked(TileModel*);
};

#endif // TILECONTROLER_H
