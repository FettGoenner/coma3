#include "saveloadgamecontroler.h"

SaveLoadGameControler::SaveLoadGameControler(SaveLoadGameModel *model, SaveLoadGameView *view, QObject *parent)
    :QObject(parent)
    , m_model(model)
    , m_view(view)
{
    this->m_view->installEventFilter(this);
    this->m_view->exec();
}
