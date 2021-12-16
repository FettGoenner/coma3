#include <QStack>

#include "gamemodel.h"
#include "tilemodel.h"
#include "endtilemodel.h"
#include "junctiontilemodel.h"
#include "cornertilemodel.h"
#include "linetilemodel.h"

GameModel::GameModel(size_t size, size_t gameSeed, QObject *parent) :
    QObject(parent)
  , _DIM(size)
  , resetVector(QVector<QVector<QVector<bool>>>(_DIM, QVector<QVector<bool>>(_DIM, {})))
  , timer(new QTimer)
  , game(QVector<QVector<TileModel*>>(_DIM, QVector<TileModel*>(_DIM, nullptr)))

{
    setGameSeed(gameSeed);
    connect(this->timer, &QTimer::timeout, this, &GameModel::setTime);
    this->timer->start(1000);

    this->initializeGame();
}

void GameModel::initializeGame(int algo)
{
    this->algoType = algo;
    // get initialized game with QStrings in a 2d Vector
    QVector<QVector<QVector<bool>>> gameAfterAlgo;
    if (algo == GameModel::Depth)
        gameAfterAlgo = this->depthAlgo();
    else if (algo == GameModel::Prim)
        gameAfterAlgo = this->primAlgo();
    else
        throw "Unknown algo type";

    // save the correct answer
    this->answer = gameAfterAlgo;
    this->clearEverything();

    // initialize different tiles on different positions
    for (size_t i = 0; i < this->_DIM; ++i) {
        for (size_t j = 0; j < this->_DIM; ++j) {
            // get the string of the nodes on the position [i,j]
            QVector<bool> nodes = gameAfterAlgo[i][j];
            TileModel *tile;
            QString tileType = TileModel::getTileTypeByVector(nodes);
            if (tileType == "EndTile") {
                tile = new EndTile(nodes);
            } else if (tileType == "JunctionTile") {
                tile = new JunctionTile(nodes);
            } else if (tileType == "CornerTile") {
                tile = new CornerTile(nodes);
            } else if (tileType == "LineTile") {
                tile = new LineTile(nodes);
            } else
                throw "This Tile does not match to any tile";

            // rotate the tile randomly
            for (int k = 0; k < getBounded(0, 4); ++k)
                tile->rotate90();

            // save tile in 2d vector playGround
            this->game[i][j] = tile;
            this->resetVector[i][j] = tile->getNodeVector();
        }
    }
    emit this->onGameInitialization(true);
}

void GameModel::setSize(size_t size)
{
    this->_DIM = size;
    for (int i = 0; i < this->game.size(); ++i) {
        for (int j = 0; j < this->game[0].size(); ++j) {
            delete this->game[i][j];
            this->game[i][j] = nullptr;
        }
    }
    this->game = QVector<QVector<TileModel*>>(this->_DIM, QVector<TileModel*>(this->_DIM, nullptr));

    this->resetVector = QVector<QVector<QVector<bool>>>(this->_DIM, QVector<QVector<bool>>(this->_DIM, {}));
}

void GameModel::setGameSeed(size_t seed)
{
    this->gen.seed(seed);
    emit this->sendGameSeed(QString::number(seed));
}

void GameModel::clearEverything()
{
    this->gameStarted = false;
    this->totalPlayTime = 0;
    this->totalSteps = 0;
    this->won = false;
}

int GameModel::getBounded(int lowest, int highest)
{
    return this->gen.bounded(lowest, highest);
}

size_t GameModel::getSize() const
{
    return this->_DIM;
}

QVector<QVector<QVector<bool>>> GameModel::depthAlgo()
{
    int m = this->_DIM;
    QVector<QVector<QVector<bool>>> v(m, QVector<QVector<bool>>(m, {0, 0, 0, 0}));
    int startTileY = getBounded(0, m), startTileX = getBounded(0, m);
    QStack<QVector<int>> tileStack;
    tileStack.push({startTileY, startTileX});
    this->startTile = {startTileY, startTileX};
    QVector<int> currentTile;
    while (!tileStack.empty()) {
        currentTile = tileStack.top();
        int currentY = currentTile[0], currentX = currentTile[1];
        QVector<QVector<int>> chooseTile{
            {currentY - 1, currentX,     TileModel::North, TileModel::South},
            {currentY,     currentX + 1, TileModel::East,  TileModel::West},
            {currentY,     currentX - 1, TileModel::West,  TileModel::East},
            {currentY + 1, currentX,     TileModel::South, TileModel::North},
        };
        for (int i = 0; i < chooseTile.size(); ++i) {
            int y = chooseTile[i][0], x = chooseTile[i][1];
            if (x < 0 || y < 0 || y >= m || x >= m || !TileModel::noNodes(v[y][x]))
                chooseTile.erase(chooseTile.begin() + (i--));
        }
        if (chooseTile.empty() || TileModel::has3Nodes(v[currentY][currentX])) {
            tileStack.pop();
            continue;
        }
        int selectIndex = getBounded(0, chooseTile.size());
        tileStack.push(chooseTile[selectIndex]);
        v[currentY][currentX][chooseTile[selectIndex][2]] = TileModel::ON;
        v[chooseTile[selectIndex][0]][chooseTile[selectIndex][1]][chooseTile[selectIndex][3]] = TileModel::ON;
    }
    return v;
}

QVector<QVector<QVector<bool>>> GameModel::primAlgo()
{
    int m = this->_DIM;
    QVector<QVector<QVector<bool>>> v(m, QVector<QVector<bool>>(m, {0, 0, 0, 0}));
    QVector<QVector<bool>> usedV(m, QVector<bool>(m, false));
    int startTileY = getBounded(0, m), startTileX = getBounded(0, m);
    this->startTile = {startTileY, startTileX};
    QVector<int> currentTile({startTileY, startTileX});
    QVector<QVector<int>> awaitTiles;
    do {
        int currentY = currentTile[0], currentX = currentTile[1];
        usedV[currentY][currentX] = true;
        QVector<QVector<int>> chooseTile{
            {currentY - 1, currentX,     currentY, currentX, TileModel::North, TileModel::South},
            {currentY,     currentX + 1, currentY, currentX, TileModel::East,  TileModel::West},
            {currentY + 1, currentX,     currentY, currentX, TileModel::South, TileModel::North},
            {currentY,     currentX - 1, currentY, currentX, TileModel::West,  TileModel::East},
        };
        for (int i = 0; i < chooseTile.size(); ++i) {
            int y = chooseTile[i][0], x = chooseTile[i][1];
            if (x < 0 || y < 0 || y >= m || x >= m || usedV[y][x]) {
                chooseTile.erase(chooseTile.begin() + i);
                --i;
            }
        }
        while (TileModel::countNodes(v[currentY][currentX]) + chooseTile.size() >= 4)
            chooseTile.erase(chooseTile.begin() + this->getBounded(0, chooseTile.size()));


        for (auto & tile : chooseTile) {
            awaitTiles.push_back(tile);
            usedV[tile[0]][tile[1]] = true;
        }

        int selectIndex = getBounded(0, awaitTiles.size());
        QVector<int> selectedTile = awaitTiles[selectIndex];
        awaitTiles.erase(awaitTiles.begin() + selectIndex);
        v[selectedTile[0]][selectedTile[1]][selectedTile[5]] = TileModel::ON;
        v[selectedTile[2]][selectedTile[3]][selectedTile[4]] = TileModel::ON;
        currentTile = selectedTile;

    } while (!awaitTiles.empty());

    return v;
}

QString GameModel::getAlgoType()
{
    if (this->algoType == GameModel::Depth)
        return "Depth";
    else if (this->algoType == GameModel::Prim)
        return "Prim";
    return "";
}

void GameModel::setTime()
{
    if (this->gameStarted) {
        ++this->totalPlayTime;
        int minute = this->totalPlayTime/60;
        int sec = this->totalPlayTime%60;
        QTime time(0, minute, sec);
        emit sendTime(time.toString("mm:ss"));
    }
}

bool GameModel::checkAnswer()
{
    if (this->won)
        return true;
    int m = this->_DIM;
    size_t countTiles = m*m - 1;
    QVector<QVector<QVector<bool>>> checkVecktor(m, QVector<QVector<bool>>(m, {false, false, false, false}));
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < m; ++j)
            checkVecktor[i][j] = this->game[i][j]->getNodeVector();

    QStack<QVector<int>> tilesStack;
    tilesStack.push(this->startTile);
    QVector<int> currentTile;
    while (!tilesStack.empty()) {
        currentTile = tilesStack.top();
        int currentY = currentTile[0], currentX = currentTile[1];
        QVector<QVector<int>> chooseNode{
            {currentY - 1, currentX,     TileModel::North, TileModel::South},
            {currentY,     currentX + 1, TileModel::East,  TileModel::West},
            {currentY + 1, currentX,     TileModel::South, TileModel::North},
            {currentY,     currentX - 1, TileModel::West,  TileModel::East},
        };
        for (int i = 0, j = 0; i < 4; ++i, ++j)
            if (!checkVecktor[currentY][currentX][i])
                chooseNode.erase(chooseNode.begin() + (j--));

        if (chooseNode.empty()) {
            tilesStack.pop();
            continue;
        }

        for (int i = 0; i < chooseNode.size(); ++i) {
            int y = chooseNode[i][0], x = chooseNode[i][1];
            if (x < 0 || y < 0 || y >= m || x >= m || !checkVecktor[y][x][chooseNode[i][3]]) {
                emit onGameStatus(false);
                return false;
            }
        }
        --countTiles;
        tilesStack.push(chooseNode[0]);
        checkVecktor[chooseNode[0][0]][chooseNode[0][1]][chooseNode[0][3]] = TileModel::OFF;
        checkVecktor[currentY][currentX][chooseNode[0][2]] = TileModel::OFF;

    }
    if (countTiles == 0){
        this->gameStarted = false;
        emit onGameStatus(true);
        this->won = true;
        return true;
    }
    else{
        emit onGameStatus(false);
        return false;
    }
}

void GameModel::resetGame()
{
    this->clearEverything();
    for (size_t i = 0; i < this->_DIM; ++i) {
        for (size_t j = 0; j < this->_DIM; ++j) {
            emit this->game[i][j]->resetedTile();
            this->game[i][j]->setNodes(this->resetVector[i][j]);
        }
    }
}

void GameModel::showSolution()
{
    this->gameStarted = false;
    for (size_t i = 0; i < this->_DIM; ++i) {
        for (size_t j = 0; j < this->_DIM; ++j) {
            emit this->game[i][j]->resetedTile();
            this->game[i][j]->setNodes(this->answer[i][j]);
        }
    }
}

void GameModel::showSolutionOnTile(TileModel *tileModel)
{
    if (!this->gameStarted) {
        this->gameStarted = true;
        emit this->gameStart();
    }
    for (size_t i = 0; i < this->_DIM; ++i) {
        for (size_t j = 0; j < this->_DIM; ++j) {
            if (this->game[i][j] == tileModel){
                this->game[i][j]->setNodes(this->answer[i][j]);
                this->checkAnswer();
            }
        }
    }
}

void GameModel::changeGameStarted(bool started)
{
    this->gameStarted = started;
}

