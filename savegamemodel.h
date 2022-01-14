#ifndef SAVEGAMEMODEL_H
#define SAVEGAMEMODEL_H

#include <QObject>
#include <QVector>
#include <QDir>
#include <QPair>

#include "networkpuzzlefile.h"

class SaveGameModel : public QObject
{
    Q_OBJECT
public:
    explicit SaveGameModel(QObject *parent = nullptr);

    QVector<QPair<NetworkPuzzleFile*, QDateTime>> getFiles() const;
private:
    QVector<QPair<NetworkPuzzleFile*, QDateTime>> fileVector;
signals:

};

#endif // SAVEGAMEMODEL_H
