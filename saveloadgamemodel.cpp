#include "saveloadgamemodel.h"

SaveLoadGameModel::SaveLoadGameModel(GameModel *gameModel, QObject *parent)
    : QObject(parent),
      currentGame(gameModel)
{
    QDir dir(NetworkPuzzleFile::GAME_FILE_PATH);
    dir.mkpath(dir.absolutePath()); // create folder games

    QStringList nameFilters;
    nameFilters << "*.json";
    QStringList files = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
    for (const QString &fileName : files) {
        NetworkPuzzleFile *npf = new NetworkPuzzleFile(fileName);
        if (npf->isValid()) {
            _fileVector.push_back(npf);
        }
    }
}

QVector<NetworkPuzzleFile*> SaveLoadGameModel::getFiles() const
{
    return _fileVector;
}

void SaveLoadGameModel::deleteFile(size_t index)
{
    _fileVector[index]->deleteFile();
    _fileVector.remove(index);
}

void SaveLoadGameModel::saveFile(int index, QString fileName)
{
    if (fileName == "")
        fileName = QDateTime::currentDateTime().toString("yy-MM-dd-hh-mm-ss");
    if (index != -1) {
        _fileVector[index]->updateData(*currentGame, fileName + ".json");
    } else {
        NetworkPuzzleFile *npf = new NetworkPuzzleFile("");
        npf->updateData(*currentGame, fileName + ".json");
        _fileVector.push_back(npf);
    }
}

void SaveLoadGameModel::loadFile(int index)
{
    NetworkPuzzleFile *file = _fileVector[index];
    currentGame->loadGame(file->getDim(), file->getSeed(), file->getGameAlgo(), file->getTotalPlayTime(), file->getTotalSteps(), file->getHintRemaining(), file->getHintedTiles(), file->getRotatedTiles());
}

