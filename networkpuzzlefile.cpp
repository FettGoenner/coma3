#include "networkpuzzlefile.h"
#include <QMessageBox>

NetworkPuzzleFile::NetworkPuzzleFile(QObject *parent) : QObject(parent)
{
}

NetworkPuzzleFile::NetworkPuzzleFile(QString &fileName, QObject *parent) : QObject(parent)
{
    this->gameFile.setFileName(fileName);
    this->loadData();
}

void NetworkPuzzleFile::loadData()
{
    if (this->gameFile.fileName().size() <= 5 || this->gameFile.fileName().right(5) != ".json")
        emit this->failedToReadFile();
    this->gameFile.open(QIODevice::ReadOnly | QIODevice::Text);
    this->gameFile.close();

}


