#include "gamemodel.h"
#include <QStack>

GameModel::GameModel(size_t row, size_t col, int gameSeed, QObject *parent) :
    QObject(parent)
  , dimX(row)
  , dimY(col)
  , game(QVector<QVector<Tile*>>(dimY, QVector<Tile*>(dimX, nullptr)))
  , resetVector(QVector<QVector<QString>>(dimY, QVector<QString>(dimX, "")))
  , timer(new QTimer)
{
    setGameSeed(gameSeed);
    connect(this->timer, &QTimer::timeout, this, [=]() {
        ++this->totalPlayTime;
        this->setTime();
    });
    this->timer->setInterval(1000);
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
    this->game = QVector<QVector<Tile*>>(this->dimY, QVector<Tile*>(this->dimX, nullptr));

    this->resetVector = QVector<QVector<QString>>(this->dimY, QVector<QString>(this->dimX, ""));
}

void GameModel::setGameSeed(int seed)
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

QVector<QVector<QString> > GameModel::depthAlgo()
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
            {currentY - 1, currentX,     Tile::North, Tile::South},
            {currentY,     currentX + 1, Tile::East,  Tile::West},
            {currentY,     currentX - 1, Tile::West,  Tile::East},
            {currentY + 1, currentX,     Tile::South, Tile::North},
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

QVector<QVector<QString> > GameModel::primAlgo()
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

bool GameModel::checkAnswer()
{
    int m = this->dimY, n = this->dimX;
    int countTiles = m*n - 1;
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
            {currentY - 1, currentX,     Tile::North, Tile::South},
            {currentY,     currentX + 1, Tile::East,  Tile::West},
            {currentY + 1, currentX,     Tile::South, Tile::North},
            {currentY,     currentX - 1, Tile::West,  Tile::East},
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
            if (x < 0 || y < 0 || y >= m || x >= n || !checkVecktor[y][x][chooseNode[i][3]])
                return false;
        }
        --countTiles;
        tilesStack.push(chooseNode[0]);
        checkVecktor[chooseNode[0][0]][chooseNode[0][1]][chooseNode[0][3]] = false;
        checkVecktor[currentY][currentX][chooseNode[0][2]] = false;

    }
    if (countTiles == 0)
        return true;
    else
        return false;
}

void GameModel::resetGame()
{
    this->gameStarted = false;
    this->totalPlayTime = 0;
    this->totalSteps = 0;
    this->totalPlayTime = 0;
    for (int i = 0; i < this->dimY; ++i) {
        for (int j = 0; j < this->dimX; ++j) {
            this->game[i][j]->setNodes(this->resetVector[i][j]);
        }
    }
}

void GameModel::showSolution()
{
    for (int i = 0; i < this->dimY; ++i) {
        for (int j = 0; j < this->dimX; ++j) {
            this->game[i][j]->setNodes(this->answer[i][j]);
        }
    }
}

