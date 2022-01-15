#ifndef SAVELOADGAMECONTROLER_H
#define SAVELOADGAMECONTROLER_H

#include <QObject>

#include "saveloadgamemodel.h"
#include "saveloadgameview.h"

class SaveLoadGameControler : public QObject
{
    Q_OBJECT
    SaveLoadGameModel *m_model;
    SaveLoadGameView *m_view;
public:
    explicit SaveLoadGameControler(SaveLoadGameModel *model, SaveLoadGameView *view, QObject *parent = nullptr);

signals:

};

#endif // SAVELOADGAMECONTROLER_H
