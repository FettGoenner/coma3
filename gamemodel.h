#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <QObject>

class GameModel : public QObject
{
    Q_OBJECT
public:
    explicit GameModel(QObject *parent = nullptr);

signals:

};

#endif // GAMEMODEL_H
