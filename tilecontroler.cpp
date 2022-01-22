#include "tilecontroler.h"
#include "tilemodel.h"
#include "tileview.h"

#include <QEvent>

TileControler::TileControler(TileModel* model, TileView* view, QObject *parent)
    : QObject(parent),
      _model(model),
      _view(view)
{
    view->installEventFilter(this);

    connect(_model, &TileModel::clicked, _view,  QOverload<>::of(&TileView::update));

    connect(_model, &TileModel::resetedTile, this, [=]() {
        _model->setBgdColor(Qt::transparent);
    });

    // set background color to yellow if the tile rotated by hint
    connect(_model, &TileModel::rotatedByHint, this, [=]() {
        _model->setBgdColor(Qt::yellow);
    });
}

void TileControler::mouseReleaseEvent(QMouseEvent*)
{
    if (!_model->timer()->isActive() && _model->clickable())
    {
        _model->rotate();
        emit _model->clicked(_model);
    }
}

bool TileControler::eventFilter(QObject*, QEvent* event)
{
    switch (event->type()) // Bestimmen des Ereignistyps
    {
    // relevante Ereignistypen behandeln:
    // cast auf speziellen Typ durchfuehren und die speziellen Event-Methoden aufrufen
    case QEvent::MouseButtonRelease:
        mouseReleaseEvent(static_cast<QMouseEvent*>(event));
        break;
    default:
        return false;
    }
    return event->isAccepted();
}
