#include "tilecontroler.h"

TileControler::TileControler(TileModel *model, TileView *view, QObject *parent)
    : QObject(parent),
      m_model(model),
      m_view(view)
{
    this->m_view->installEventFilter(this);
}

void TileControler::mouseReleaseEvent(QMouseEvent *)
{
    if (this->m_view->canBeClicked) {
        this->m_view->rotateWithAnimation(90);

        // send signal clicked()
        emit this->m_view->clicked();
        emit this->clicked(this->m_model);
    }
}

bool TileControler::eventFilter(QObject *, QEvent *event)
{
    switch (event->type())
    {
    case QEvent::MouseButtonRelease:
        mouseReleaseEvent(static_cast<QMouseEvent*>(event));
        break;
    default:
        return false;
    }
    return event->isAccepted();
}


