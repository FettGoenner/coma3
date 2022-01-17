#include "networkpuzzlefile.h"

const QString NetworkPuzzleFile::GAME_FILE_PATH = "games/";

NetworkPuzzleFile::NetworkPuzzleFile(QObject *parent) : QObject(parent)
{
}

NetworkPuzzleFile::NetworkPuzzleFile(const QString &fileName, QObject *parent)
    : QObject(parent),
      fileName(fileName)
{
    if (fileName != "") {
        this->gameFile.setFileName(NetworkPuzzleFile::GAME_FILE_PATH + fileName);
        this->dataValid = this->isValid();
        this->loadData();
    }
}

void NetworkPuzzleFile::loadData()
{
    if (!this->dataValid)
        return;

    /* dim, seed, gameAlgo, totalPlayTime, totalSteps, hintRamaining, hintedTiles, changedTiles */
    // get data and check valid
    QJsonValue dim = this->gameObj["dim"];
    this->dim = size_t(dim.toInt());

    QJsonValue seed = this->gameObj["seed"];
    this->seed = size_t(seed.toInt());

    QJsonValue gameAlgo = this->gameObj["gameAlgo"];
    this->gameAlgo = gameAlgo.toString();

    QJsonValue totalPlayTime = this->gameObj["totalPlayTime"];
    this->totalPlayTime = size_t(totalPlayTime.toInt());

    QJsonValue totalSteps = this->gameObj["totalSteps"];
    this->totalSteps = size_t(totalSteps.toInt());

    QJsonValue hintRemaining = this->gameObj["hintRemaining"];
    this->hintRemaining = size_t(hintRemaining.toInt());

    QJsonValue hintedTiles = this->gameObj["hintedTiles"];
    QJsonArray hintedTilesArray = hintedTiles.toArray();
    foreach (const QJsonValue &value, hintedTilesArray){
        QString valueString = value.toString();
        QPair<size_t, size_t> pos(valueString[0].digitValue(), valueString[1].digitValue());
        this->hintedTiles.push_back(pos);
    }

    QJsonValue rotatedTiles = this->gameObj["rotatedTiles"];
    QJsonArray changedTilesArray = rotatedTiles.toArray();

    foreach (const auto &value, changedTilesArray) {
        QJsonObject valueObject = value.toObject();

        QJsonValue position = valueObject["position"];
        QString posString = position.toString();
        QPair<size_t, size_t> pos(posString[0].digitValue(), posString[1].digitValue());

        QVector<bool> nds(4, false);
        QJsonValue nodes = valueObject["nodes"];
        QString nodesString = nodes.toString();
        for (int i = 0; i < 4; ++i) {
            QChar c = nodesString[i];
            nds[i] = c == '1' ? true : false;
        }
        this->rotatedTiles.push_back({pos, nds});
    }
}

bool NetworkPuzzleFile::writeData()
{
    if (!this->gameFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QJsonDocument saveDoc(this->gameObj);
    this->gameFile.write(saveDoc.toJson());
    this->gameFile.close();

    return true;

}

bool NetworkPuzzleFile::isValid()
{
    bool dataIsValid = true;
    if (this->gameFile.fileName().size() <= 5 || this->gameFile.fileName().right(5) != ".json")
        dataIsValid = false;

    // read json file
    QString  gameDetail;
    this->gameFile.open(QIODevice::ReadOnly | QIODevice::Text);
    gameDetail = this->gameFile.readAll();
    this->gameFile.close();

    this->gameObj = QJsonDocument::fromJson(gameDetail.toUtf8()).object();

    /* dim, seed, gameAlgo, totalPlayTime, totalSteps, hintRamaining, hintedTiles, changedTiles */
    // get data and check valid


    QJsonValue dim = this->gameObj["dim"];
    if (!dim.isDouble() || size_t(dim.toInt()) > GameModel::MAX_SIZE || size_t(dim.toInt()) < GameModel::MIN_SIZE)
        dataIsValid = false;

    QJsonValue seed = this->gameObj.value("seed");
    seed.isDouble();
    seed.toInt();
    if (!seed.isDouble() || seed.toInt() > INT_MAX || seed.toInt() < 0)
        dataIsValid = false;
    // true

    QJsonValue gameAlgo = this->gameObj["gameAlgo"];
    if (!gameAlgo.isString() || (gameAlgo.toString() != "Depth" && gameAlgo.toString() != "Prim"))
        dataIsValid = false;

    QJsonValue totalPlayTime = this->gameObj["totalPlayTime"];
    if (!totalPlayTime.isDouble() || totalPlayTime.toInt() > INT_MAX || totalPlayTime.toInt() < 0)
        dataIsValid = false;


    QJsonValue totalSteps = this->gameObj["totalSteps"];
    if (!totalSteps.isDouble() || totalSteps.toInt() > INT_MAX || totalSteps.toInt() < 0)
        dataIsValid = false;

    QJsonValue hintRemaining = this->gameObj["hintRemaining"];
    if (!hintRemaining.isDouble() || size_t(hintRemaining.toInt()) > GameModel::HINTLIMIT || hintRemaining.toInt() < 0)
        dataIsValid = false;

    QJsonValue hintedTiles = this->gameObj["hintedTiles"];
    if (hintedTiles.isArray()) {
        QJsonArray hintedTilesArray = hintedTiles.toArray();
        for (const QJsonValue &value : hintedTilesArray){
            if (!value.isString() || value.toString().size() != 2)
                dataIsValid = false;

            QString valueString = value.toString();
            if (valueString[0].digitValue() >= dim.toInt() || valueString[1].digitValue() >= dim.toInt())
                dataIsValid = false;
        }
    } else
        dataIsValid = false;

    QJsonValue rotatedTiles = this->gameObj["rotatedTiles"];
    if (rotatedTiles.isArray()){
        QJsonArray changedTilesArray = rotatedTiles.toArray();
        for (const auto &value : changedTilesArray){
            if (!value.isObject())
                dataIsValid = false;

            QJsonObject valueObject = value.toObject();

            QJsonValue position = valueObject["position"];
            if (!position.isString() || position.toString().size() != 2)
                dataIsValid = false;

            QString posString = position.toString();
            if (posString[0].digitValue() >= dim.toInt() || posString[1].digitValue() >= dim.toInt())
                dataIsValid = false;

            QJsonValue nodes = valueObject["nodes"];
            if (!nodes.isString() || nodes.toString().size() != 4)
                dataIsValid = false;
            QString nodesString = nodes.toString();
            for (int i = 0; i < 4; ++i) {
                QChar c = nodesString[i];
                int isActive = c.digitValue();
                if (isActive != 0 && isActive != 1)
                    dataIsValid = false;
            }
        }
    } else
        dataIsValid = false;

    return dataIsValid;
}

size_t NetworkPuzzleFile::getDim() const
{
    return this->dim;
}

size_t NetworkPuzzleFile::getSeed() const
{
    return this->seed;
}

QString NetworkPuzzleFile::getGameAlgo() const
{
    return this->gameAlgo;
}

size_t NetworkPuzzleFile::getTotalPlayTime() const
{
    return this->totalPlayTime;
}

size_t NetworkPuzzleFile::getTotalSteps() const
{
    return this->totalSteps;
}

size_t NetworkPuzzleFile::getHintRemaining() const
{
    return this->hintRemaining;
}

QVector<QPair<size_t, size_t>> NetworkPuzzleFile::getHintedTiles() const
{
    return this->hintedTiles;
}

QVector<QPair<QPair<size_t, size_t>, QVector<bool>>> NetworkPuzzleFile::getRotatedTiles() const
{
    return this->rotatedTiles;
}

QString NetworkPuzzleFile::getFileName() const
{
    return this->fileName;
}

QDateTime NetworkPuzzleFile::lastModified() const
{
    QFileInfo fileInfo(NetworkPuzzleFile::GAME_FILE_PATH + this->fileName);
    return fileInfo.lastModified();

}

void NetworkPuzzleFile::updateData(GameModel &gameModel, QString fileName)
{
    this->dim = gameModel.getSize();
    QJsonObject obj;
    obj["dim"] = QJsonValue(static_cast<int>(this->dim));

    this->seed = gameModel.getSeed();
    obj["seed"] = QJsonValue(static_cast<int>(gameModel.getSeed()));

    this->gameAlgo = gameModel.getAlgo();
    obj["gameAlgo"] = QJsonValue(gameAlgo);

    this->totalPlayTime = gameModel.getTotalPlayTime();
    obj["totalPlayTime"] = QJsonValue(static_cast<int>(this->totalPlayTime));

    this->totalSteps = gameModel.getTotalSteps();
    obj["totalSteps"] = QJsonValue(static_cast<int>(this->totalSteps));

    this->hintRemaining = gameModel.getHintRamaining();
    obj["hintRemaining"] = QJsonValue(static_cast<int>(this->hintRemaining));

    this->hintedTiles = gameModel.getHintedTiles();
    QJsonArray hintedTilesArray;
    for (const auto &pair : this->hintedTiles)
        hintedTilesArray.append(QString("%1%2").arg(pair.first).arg(pair.second));
    obj["hintedTiles"] = hintedTilesArray;

    this->rotatedTiles = gameModel.getRotatedTiles();
     QJsonArray rotatedTilesArray;
     for (const auto &value : this->rotatedTiles) {
         QString position = QString("%1%2").arg(value.first.first).arg(value.first.second);
         QString nodes("");
         for (bool b : value.second)
             nodes += b ? "1" : "0";
         QJsonObject tiles;
         tiles["position"] = QJsonValue(position);
         tiles["nodes"] = QJsonValue(nodes);
         rotatedTilesArray.append(tiles);
     }
     obj["rotatedTiles"] = rotatedTilesArray;

     if (fileName != ".json") {
         this->fileName = fileName;
         this->gameFile.setFileName(NetworkPuzzleFile::GAME_FILE_PATH + this->fileName);
     }

     this->gameObj = obj;
     this->writeData();
     this->isValid();

}

void NetworkPuzzleFile::deleteFile()
{
    this->gameFile.remove();
}


