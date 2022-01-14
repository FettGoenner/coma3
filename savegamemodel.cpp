#include "savegamemodel.h"

SaveGameModel::SaveGameModel(QObject *parent) : QObject(parent)
{
    QDir dir("games/");
    QStringList nameFilters;
    nameFilters << "*.json";
    QStringList files = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
    for (const QString &fileName : files) {
         NetworkPuzzleFile *npf = new NetworkPuzzleFile(fileName);
         if (npf->isValid()) {
             QFileInfo fileInfo("games/" + fileName);
             QDateTime lastModifiedTime = fileInfo.lastModified();
             this->fileVector.push_back({npf, lastModifiedTime});
         }
    }
}

QVector<QPair<NetworkPuzzleFile*, QDateTime>> SaveGameModel::getFiles() const
{
    return this->fileVector;
}

