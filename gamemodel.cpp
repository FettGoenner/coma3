#include <QStack>

#include "gamemodel.h"
#include "tilemodel.h"
#include "endtilemodel.h"
#include "junctiontilemodel.h"
#include "cornertilemodel.h"
#include "linetilemodel.h"

GameModel::GameModel(size_t row, size_t col, size_t gameSeed, QObject *parent) :
    QObject(parent)
  , dimX(row)
  , dimY(col)
  , game(QVector<QVector<TileModel*>>(dimY, QVector<TileModel*>(dimX, nullptr)))
  , resetVector(QVector<QVector<QString>>(dimY, QVector<QString>(dimX, "")))
  , timer(new QTimer)
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
    QVector<QVector<QString>> gameAfterAlgo;
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
    for (size_t i = 0; i < this->dimY; ++i) {
        for (size_t j = 0; j < this->dimX; ++j) {
            // get the string of the nodes on the position [i,j]
            QString nodes = gameAfterAlgo[i][j];
            TileModel *tile;
            if (nodes.size() == 1) {
                tile = new EndTile(nodes);
            } else if (nodes.size() == 3) {
                tile = new JunctionTile(nodes);
            } else {
                int first = nodes[0].digitValue(), second = nodes[1].digitValue();
                if (first > second) {
                    int temp = first;
                    first = second;
                    second = temp;
                }
                if (second - first == 1 || second - first == 3)
                    tile = new CornerTile(nodes);

                else
                    tile = new LineTile(nodes);
            }
            // rotate the tile randomly
            for (int k = 0; k < getBounded(0, 4); ++k)
                tile->rotate90();

            // save tile in 2d vector playGround
            this->game[i][j] = tile;
            this->resetVector[i][j] = tile->getNodeString();
        }
    }
    emit this->onGameInitialization(true);
}

void GameModel::setSize(int row, int col)
{
    this->dimY = row;
    this->dimX = col;
    for (int i = 0; i < this->game.size(); ++i) {
        for (int j = 0; j < this->game[0].size(); ++j) {
            delete this->game[i][j];
            this->game[i][j] = nullptr;
        }
    }
    this->game = QVector<QVector<TileModel*>>(this->dimY, QVector<TileModel*>(this->dimX, nullptr));

    this->resetVector = QVector<QVector<QString>>(this->dimY, QVector<QString>(this->dimX, ""));
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
}

int GameModel::getBounded(int lowest, int highest)
{
    return this->gen.bounded(lowest, highest);
}

size_t GameModel::getRow() const
{
    return this->dimY;
}

size_t GameModel::getCol() const
{
    return this->dimX;
}

QVector<QVector<QString>> GameModel::depthAlgo()
{
    int m = this->dimY, n = this->dimX;
    QVector<QVector<QString>> v(m, QVector<QString>(n, ""));
    int startTileY = getBounded(0, m), startTileX = getBounded(0, n);
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
            if (x < 0 || y < 0 || y >= m || x >= n || !v[y][x].isEmpty())
                chooseTile.erase(chooseTile.cbegin() + (i--));
        }
        if (chooseTile.empty() || v[currentY][currentX].size() == 3) {
            tileStack.pop();
            continue;
        }
        int selectIndex = getBounded(0, chooseTile.size());
        tileStack.push(chooseTile[selectIndex]);
        v[currentY][currentX] += QString::number(chooseTile[selectIndex][2]);
        v[chooseTile[selectIndex][0]][chooseTile[selectIndex][1]] +=  QString::number(chooseTile[selectIndex][3]);
    }
    return v;
}

QVector<QVector<QString>> GameModel::primAlgo()
{
    int m = this->dimY, n = this->dimX;
    QVector<QVector<QString>> v(m, QVector<QString>(n, ""));
    QVector<QVector<bool>> usedV(m, QVector<bool>(n, false));
    int startTileY = getBounded(0, m), startTileX = getBounded(0, n);
    this->startTile = {startTileY, startTileX};
    QVector<int> currentTile({startTileY, startTileX});
    QVector<QVector<int>> awaitTiles;
    do {
        int currentY = currentTile[0], currentX = currentTile[1];
        usedV[currentY][currentX] = true;
        QVector<QVector<int>> chooseTile{
            {currentY - 1, currentX,     currentY, currentX, 0, 2},
            {currentY,     currentX + 1, currentY, currentX, 1, 3},
            {currentY + 1, currentX,     currentY, currentX, 2, 0},
            {currentY,     currentX - 1, currentY, currentX, 3, 1},
        };
        for (int i = 0; i < chooseTile.size(); ++i) {
            int y = chooseTile[i][0], x = chooseTile[i][1];
            if (x < 0 || y < 0 || y >= m || x >= n || usedV[y][x]) {
                chooseTile.erase(chooseTile.cbegin() + i);
                --i;
            }
        }
        while (v[currentY][currentX].size() + chooseTile.size() >= 4)
            chooseTile.erase(chooseTile.cbegin() + this->getBounded(0, chooseTile.size()));


        for (auto & tile : chooseTile) {
            awaitTiles.push_back(tile);
            usedV[tile[0]][tile[1]] = true;
        }

        int selectIndex = getBounded(0, awaitTiles.size());
        QVector<int> selectedTile = awaitTiles[selectIndex];
        awaitTiles.erase(awaitTiles.cbegin() + selectIndex);
        v[selectedTile[0]][selectedTile[1]] += QString::number(selectedTile[5]);
        v[selectedTile[2]][selectedTile[3]] += QString::number(selectedTile[4]);
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
    int m = this->dimY, n = this->dimX;
    size_t countTiles = m*n - 1;
    QVector<QVector<QVector<bool>>> checkVecktor(m, QVector<QVector<bool>>(n, {false, false, false, false}));
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j)
            checkVecktor[i][j] = this->game[i][j]->nodes;

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
                chooseNode.erase(chooseNode.cbegin() + (j--));

        if (chooseNode.empty()) {
            tilesStack.pop();
            continue;
        }

        for (int i = 0; i < chooseNode.size(); ++i) {
            int y = chooseNode[i][0], x = chooseNode[i][1];
            if (x < 0 || y < 0 || y >= m || x >= n || !checkVecktor[y][x][chooseNode[i][3]]) {
                emit onGameStatus(false);
                return false;
            }
        }
        --countTiles;
        tilesStack.push(chooseNode[0]);
        checkVecktor[chooseNode[0][0]][chooseNode[0][1]][chooseNode[0][3]] = false;
        checkVecktor[currentY][currentX][chooseNode[0][2]] = false;

    }
    if (countTiles == 0){
        this->gameStarted = false;
        emit onGameStatus(true);
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
    for (size_t i = 0; i < this->dimY; ++i) {
        for (size_t j = 0; j < this->dimX; ++j) {
            emit this->game[i][j]->resetTile();
            this->game[i][j]->setNodes(this->resetVector[i][j]);
        }
    }
    emit this->onGameStatus(false);
}

void GameModel::showSolution()
{
    this->gameStarted = false;
    for (size_t i = 0; i < this->dimY; ++i) {
        for (size_t j = 0; j < this->dimX; ++j) {
            emit this->game[i][j]->resetTile();
            this->game[i][j]->setNodes(this->answer[i][j]);
        }
    }
    emit this->onGameStatus(true);
}

void GameModel::startHint()
{
    for (size_t i = 0; i < this->dimY; ++i) {
        for (size_t j = 0; j < this->dimX; ++j) {
            emit this->game[i][j]->startHint();
        }
    }
}

void GameModel::endHint()
{
    for (size_t i = 0; i < this->dimY; ++i) {
        for (size_t j = 0; j < this->dimX; ++j) {
            emit this->game[i][j]->endHint();
        }
    }
}

void GameModel::showSolutionOnTile(TileModel *tileModel)
{
    for (size_t i = 0; i < this->dimY; ++i) {
        for (size_t j = 0; j < this->dimX; ++j) {
            emit this->game[i][j]->endHint();
            if (this->game[i][j] == tileModel)
                this->game[i][j]->setNodes(this->answer[i][j]);
        }
    }
}

