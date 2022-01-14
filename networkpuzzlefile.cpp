#include "networkpuzzlefile.h"

NetworkPuzzleFile::NetworkPuzzleFile(QObject *parent) : QObject(parent)
{
}

NetworkPuzzleFile::NetworkPuzzleFile(const QString &fileName, QObject *parent)
    : QObject(parent),
      fileName(fileName)
{
    this->gameFile.setFileName("games/" + fileName);

    this->dataValid = this->isValid();
    this->loadData();
}

void NetworkPuzzleFile::loadData()
{
    if (!this->dataValid)
        return;

    // read json file
    QString gameDetail;
    this->gameFile.open(QIODevice::ReadOnly | QIODevice::Text);
    gameDetail = this->gameFile.readAll();
    this->gameFile.close();
    QJsonObject gameObj = QJsonDocument::fromJson(gameDetail.toUtf8()).object();

    /* dim, seed, gameAlgo, totalPlayTime, totalSteps, hintRamaining, hintedTiles, changedTiles */
    // get data and check valid
    QJsonValue dim = gameObj["dim"];
    this->dim = size_t(dim.toInt());

    QJsonValue seed = gameObj["seed"];
    this->seed = size_t(seed.toInt());

    QJsonValue gameAlgo = gameObj["gameAlgo"];
    this->gameAlgo = gameAlgo.toString();

    QJsonValue totalPlayTime = gameObj["totalPlayTime"];
    this->totalPlayTime = size_t(totalPlayTime.toInt());

    QJsonValue totalSteps = gameObj["totalSteps"];
    this->totalSteps = size_t(totalSteps.toInt());

    QJsonValue hintRemaining = gameObj["hintRemaining"];
    this->hintRemaining = size_t(hintRemaining.toInt());

    QJsonValue hintedTiles = gameObj["hintedTiles"];
    QJsonArray hintedTilesArray = hintedTiles.toArray();
    for (const QJsonValue &value : hintedTilesArray){
        QString valueString = value.toString();
        this->hintedTiles.push_back({valueString[0].digitValue(), valueString[1].digitValue()});
    }

    QJsonValue rotatedTiles = gameObj["rotatedTiles"];
    QJsonArray changedTilesArray = rotatedTiles.toArray();

    for (const auto &value : changedTilesArray) {
        QJsonObject valueObject = value.toObject();
        QPair<size_t, size_t> pos;
        QJsonValue position = valueObject["position"];
        QString posString = position.toString();
        pos = {posString[0].digitValue(), posString[1].digitValue()};

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

void NetworkPuzzleFile::writeData(GameModel *gameModel)
{
    this->dim = gameModel->getSize();
    this->seed = gameModel->getGameSeed();
    this->gameAlgo = gameModel->getAlgoType();
    this->totalPlayTime = gameModel->getTotalPlayTime();
    this->totalSteps = gameModel->getTotalSteps();
    this->hintRemaining = gameModel->getHintRamaining();
    this->hintedTiles = gameModel->getHintedTiles();
    this->rotatedTiles = gameModel->getRotatedTiles();
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

    QJsonObject gameObj = QJsonDocument::fromJson(gameDetail.toUtf8()).object();

    /* dim, seed, gameAlgo, totalPlayTime, totalSteps, hintRamaining, hintedTiles, changedTiles */
    // get data and check valid


    QJsonValue dim = gameObj["dim"];
    if (!dim.isDouble() || size_t(dim.toInt()) > GameModel::MAX_SIZE || size_t(dim.toInt()) < GameModel::MIN_SIZE)
        dataIsValid = false;

    QJsonValue seed = gameObj.value("seed");
    seed.isDouble();
    seed.toInt();
    if (!seed.isDouble() || seed.toInt() > INT_MAX || seed.toInt() < 0)
        dataIsValid = false;
    // true

    QJsonValue gameAlgo = gameObj["gameAlgo"];
    if (!gameAlgo.isString() || (gameAlgo.toString() != "Depth" && gameAlgo.toString() != "Prim"))
        dataIsValid = false;

    QJsonValue totalPlayTime = gameObj["totalPlayTime"];
    if (!totalPlayTime.isDouble() || totalPlayTime.toInt() > INT_MAX || totalPlayTime.toInt() < 0)
        dataIsValid = false;


    QJsonValue totalSteps = gameObj["totalSteps"];
    if (!totalSteps.isDouble() || totalSteps.toInt() > INT_MAX || totalSteps.toInt() < 0)
        dataIsValid = false;

    QJsonValue hintRemaining = gameObj["hintRemaining"];
    if (!hintRemaining.isDouble() || size_t(hintRemaining.toInt()) > GameModel::HINT_LIMIT || hintRemaining.toInt() < 0)
        dataIsValid = false;

    QJsonValue hintedTiles = gameObj["hintedTiles"];
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

    QJsonValue rotatedTiles = gameObj["rotatedTiles"];
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


