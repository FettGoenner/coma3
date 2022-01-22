#ifndef SAVELOADGAMEMODEL_H
#define SAVELOADGAMEMODEL_H

#include <QObject>
#include <QVector>
#include <QDir>

#include "networkpuzzlefile.h"
#include "gamemodel.h"

class SaveLoadGameModel : public QObject
{
    Q_OBJECT
public:
    explicit SaveLoadGameModel(GameModel *gameModel, QObject *parent = nullptr);

    QVector<NetworkPuzzleFile*> getFiles() const;
    GameModel *currentGame;

private:
    QVector<NetworkPuzzleFile*> _fileVector;
signals:

public slots:
    void deleteFile(size_t index);
    void saveFile(int index, QString fileName);
    void loadFile(int index);
};

#endif // SAVELOADGAMEMODEL_H
