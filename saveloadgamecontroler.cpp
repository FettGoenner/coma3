#include "saveloadgamecontroler.h"

SaveLoadGameControler::SaveLoadGameControler(SaveLoadGameModel *model, SaveLoadGameView *view, QObject *parent)
    :QObject(parent)
    , m_model(model)
    , m_view(view)
{
    this->m_view->installEventFilter(this);

    connect(this->m_view, &SaveLoadGameView::deleteFile, this->m_model, &SaveLoadGameModel::deleteFile);
    connect(this->m_view, &SaveLoadGameView::saveBtnClicked, this->m_model, &SaveLoadGameModel::saveFile);
    connect(this->m_view, &SaveLoadGameView::loadBtnClicked, this->m_model, &SaveLoadGameModel::loadFile);
    this->m_view->exec();
}
