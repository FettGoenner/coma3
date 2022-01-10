#include "gamecontroler.h"

#include <QEvent>
#include <QMouseEvent>
#include <QKeyEvent>

#include "gamemodel.h"
#include "gameview.h"

GameControler::GameControler(GameModel& model, GameView& view, QObject * parent)
    : QObject(parent),
      gameModel(model),
      gameView(view)
{
    view.installEventFilter(this);
}

void GameControler::mousePressEvent(QMouseEvent* event)
{
    //  What to do, if mouse is pressed
    /*
    const double breite = m_view.width();
    const double hoehe = m_view.height();

    if (event->button() == Qt::LeftButton)
    {
        m_model.quadratHinzufuegen(event->x()/breite, event->y()/hoehe);
    }
    */
}

void GameControler::keyPressEvent(QKeyEvent* event)
{
    //  What to do if ey is pressed
    /*
    switch (event->key())
    {
    case Qt::Key_Delete:
        m_model.quadratEntfernen();
        break;
   // ggf. weitere Tasten
    default:
        break;
    }
    */
}

// Eventfilter for catching events from the tileview :
// MVC: the events are processed by the controler
bool QuadrateControler::eventFilter(QObject* /*watched*/, QEvent* event)
{
    switch (event->type()) // Bestimmen des Ereignistyps
    {
        // relevante Ereignistypen behandeln:
        // cast auf speziellen Typ durchfuehren und die speziellen Event-Methoden aufrufen
    case QEvent::MouseButtonPress:
        mousePressEvent(dynamic_cast<QMouseEvent*>(event));
        break;
    case QEvent::KeyPress:
        keyPressEvent(dynamic_cast<QKeyEvent*>(event));
        break;
    default:
        return false;
    }
    return event->isAccepted();
}

