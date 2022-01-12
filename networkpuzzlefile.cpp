#include <QMessageBox>
#include <QDebug>
#include <QStandardPaths>

#include "networkpuzzlefile.h"
NetworkPuzzleFile::NetworkPuzzleFile(QObject *parent) : QObject(parent)
{
}

NetworkPuzzleFile::NetworkPuzzleFile(const QString &fileName, QObject *parent) : QObject(parent)
{
    this->gameFile.setFileName(fileName);
    this->loadData();
}

void NetworkPuzzleFile::loadData()
{
    if (this->gameFile.fileName().size() <= 5 || this->gameFile.fileName().right(5) != ".json"){
        emit this->failedToReadFile();
        return;
    }
    QString val;
    this->gameFile.open(QIODevice::ReadOnly | QIODevice::Text);
    val = this->gameFile.readAll();
    this->gameFile.close();
    qDebug() << val;
}


