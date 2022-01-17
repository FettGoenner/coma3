#include <QStack>

#include "gamemodel.h"
#include "endtile.h"
#include "junctiontile.h"
#include "cornertile.h"
#include "linetile.h"
#include "tileview.h"
#include "tilecontroler.h"

GameModel::GameModel(size_t size, size_t gameSeed, QObject *parent)
    : QObject(parent)
    , _DIM(size)
    , _resetVector(QVector<QVector<QVector<bool>>>(_DIM, QVector<QVector<bool>>(_DIM, {})))
    , _timer(new QTimer)
    , gridLayout(nullptr)
    , _game(QVector<QVector<TileModel*>>(_DIM, QVector<TileModel*>(_DIM, nullptr)))
{
    setSeed(gameSeed);
    connect(this->_timer, &QTimer::timeout, this, &GameModel::setTime);
    this->_timer->start(1000);

    this->initGame();
}


void GameModel::clear()
{
    if (gridLayout != nullptr)
    {
        QLayoutItem* item;
        while ((item = gridLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete gridLayout;
        gridLayout = nullptr;
    }

    // create gridlayout
    gridLayout = new QGridLayout;
    gridLayout->setSpacing(0);
    gridLayout->setContentsMargins(0, 0, 0, 0);
}

void GameModel::clearEverything(bool clearLayout)
{
    _started = false;
    _totalTime = 0;
    _totalSteps = 0;
    _hintCount = 0;
    _hintedTiles = QVector<QPair<size_t, size_t>>();
    _rotatedTiles = QVector<QPair<QPair<size_t, size_t>, QVector<bool>>>();
    if (clearLayout)
        this->clear();
}

//  INIT GAME
void GameModel::initGame(GameModel::GameType algo)
{
    _algo = algo;
    // get initialized game with QStrings in a 2d Vector
    QVector<QVector<QVector<bool>>> gameAfterAlgo;
    if (algo == GameModel::Depth)
        gameAfterAlgo = this->depthAlgo();
    else if (algo == GameModel::Prim)
        gameAfterAlgo = this->primAlgo();
    else
        throw "Unknown algo type";

    // save the correct answer
    _answer = gameAfterAlgo;
    this->clearEverything(true);
    // initialize different tiles on different positions
    for (size_t i = 0; i < _DIM; ++i) {
        for (size_t j = 0; j < this->_DIM; ++j) {
            // get the string of the nodes on the position [i,j]
            QVector<bool> nodes = gameAfterAlgo[i][j];
            TileModel *tileModel;
            TileModel::TileNames tileType = TileModel::typeByVector(nodes);
            if (tileType == TileModel::EndTile) {
                tileModel = new EndTile(nodes);
            } else if (tileType == TileModel::JunctionTile) {
                tileModel = new JunctionTile(nodes);
            } else if (tileType == TileModel::CornerTile) {
                tileModel = new CornerTile(nodes);
            } else if (tileType == TileModel::LineTile) {
                tileModel = new LineTile(nodes);
            } else
                throw "This Tile does not match to any tile";

            // save tile in 2d vector playGround
            _game[i][j] = tileModel;

            // create TileView
            TileView *tileView = new TileView(tileModel, Qt::blue);

            gridLayout->addWidget(tileView, i, j);

            // rotate the tile randomly
            tileModel->rotate(getBounded(0, 4));
            _resetVector[i][j] = tileModel->nodes();

            new TileControler(tileModel, tileView, this);

            // set steps after clicked a tile
            connect(tileModel, &TileModel::clicked, this, &GameModel::setSteps);

            // check answer after clicked a tile
            connect(tileModel, &TileModel::clicked, this, &GameModel::checkAnswer);

            // set tile's color between green and blue
            connect(this, &GameModel::onGameStatus, tileModel, &TileModel::connected);

            connect(tileModel, &TileModel::clicked, this, &GameModel::tileRotatedByView);
        }
    }
    emit this->onGameInit();
    emit this->hintSuccessed(GameModel::HINTLIMIT);
}   //  INIT GAME

void GameModel::setSize(size_t size)
{
    _DIM = size;
    for (int i = 0; i < _game.size(); ++i) {
        for (int j = 0; j < _game[0].size(); ++j) {
            delete _game[i][j];
            _game[i][j] = nullptr;
        }
    }
    _game = QVector<QVector<TileModel*>>(_DIM, QVector<TileModel*>(_DIM, nullptr));
    _resetVector = QVector<QVector<QVector<bool>>>(_DIM, QVector<QVector<bool>>(_DIM, {}));
}

QString GameModel::getAlgo()
{
    if (_algo == GameModel::Depth)
        return "Depth";
    else if (_algo == GameModel::Prim)
        return "Prim";
    return "";
}

void GameModel::setTime()
{
    if (_started) {
        ++_totalTime;
        int minute = _totalTime/60;
        int sec = _totalTime%60;
        QTime time(0, minute, sec);
        emit sendTime(time.toString("mm:ss"));
    }
}

bool GameModel::checkAnswer()
{
    int m = _DIM;
    size_t countTiles = m*m - 1;
    QVector<QVector<QVector<bool>>> checkVecktor(m, QVector<QVector<bool>>(m, {false, false, false, false}));
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < m; ++j)
            checkVecktor[i][j] = _game[i][j]->nodes();

    QStack<QVector<int>> tilesStack;
    tilesStack.push(_startTile);
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
        _started = false;
        emit onGameStatus(true);
        return true;
    }
    else{
        emit onGameStatus(false);
        return false;
    }
}

void GameModel::showSolution()
{
    _started = false;
    for (size_t i = 0; i < this->_DIM; ++i) {
        for (size_t j = 0; j < this->_DIM; ++j) {
            emit _game[i][j]->resetedTile();
            _game[i][j]->setNodes(_answer[i][j]);
            tileRotatedByView(_game[i][j]);
        }
    }
    emit onGameStatus(true);
}

void GameModel::showSolutionOnRandomTile()
{
    if (_hintCount >= GameModel::HINTLIMIT)
        return;

    if (!_started) {
        _started = true;
        emit this->gameStart();
    }
    QVector<QPair<size_t, size_t>> unSolvedTiles;

    // set all unsolved tiles in a vector
    for (size_t i = 0; i < this->_DIM; ++i)
        for (size_t j = 0; j < this->_DIM; ++j)
            if (this->_game[i][j]->nodes() != this->_answer[i][j])
                unSolvedTiles.push_back({i, j});
    // get the position randomly
    QPair<size_t, size_t> position = unSolvedTiles[this->getBounded(0, unSolvedTiles.size())];
    this->_game[position.first][position.second]->setNodes(this->_answer[position.first][position.second]);

    this->_hintedTiles.push_back(position);
    emit this->_game[position.first][position.second]->rotatedByHint();
    emit this->hintSuccessed(GameModel::HINTLIMIT - ++this->_hintCount);
    this->checkAnswer();
}

void GameModel::changeGameStarted(bool started)
{
    _started = started;
    if (_started == false)
        emit paused();
}

void GameModel::loadGame(const size_t dim, const size_t seed, const QString &gameAlgo, const size_t totalPlayTime, const size_t totalSteps, const size_t hintRamaining, const QVector<QPair<size_t, size_t>> &hintedTiles, const QVector<QPair<QPair<size_t, size_t>, QVector<bool>>> &rotatedTiles)
{
    this->setSize(dim);

    this->setSeed(seed);

    GameModel::GameType algoType = gameAlgo == "Depth" ? GameModel::Depth : GameModel::Prim;

    this->initGame(algoType);

    this->setTotalTime(totalPlayTime);

    this->setStep(totalSteps);

    this->_hintCount = GameModel::HINTLIMIT - hintRamaining;
    emit this->hintSuccessed(hintRamaining);

    for (const auto &pos : hintedTiles) {
        this->_game[pos.first][pos.second]->setNodes(this->_answer[pos.first][pos.second]);
        emit this->_game[pos.first][pos.second]->rotatedByHint();
    }
    this->_hintedTiles = hintedTiles;

    for (const auto &pair : rotatedTiles) {
        QPair<size_t, size_t> pos = pair.first;
        QVector<bool> nodes = pair.second;

        this->_game[pos.first][pos.second]->setNodes(nodes);
    }
    this->_rotatedTiles = rotatedTiles;
    this->checkAnswer();
}

void GameModel::resetGame()
{
    this->clearEverything();
    for (size_t i = 0; i < this->_DIM; ++i) {
        for (size_t j = 0; j < this->_DIM; ++j) {
            emit this->_game[i][j]->resetedTile();
            this->_game[i][j]->setNodes(this->_resetVector[i][j]);
        }
    }
    emit onGameStatus(false);
    emit this->hintSuccessed(GameModel::HINTLIMIT);
}

void GameModel::tileRotatedByView(TileModel *tile)
{
    for (size_t i = 0; i < this->_DIM; ++i) {
        for (size_t j = 0; j < this->_DIM; ++j) {
            if (this->_game[i][j] == tile) {
                auto it = this->_rotatedTiles.begin();
                while (it != this->_rotatedTiles.end()) { // check if tile's position already in the vector
                    QPair<size_t, size_t> pos = it->first;
                    if (pos.first == i && pos.second == j) { // update the nodes of the tile
                        it->second = tile->nodes();
                        break;
                    }
                    ++it;
                }
                if (it == this->_rotatedTiles.end()) // if the tile not exsist in vector rotatedTiles
                    this->_rotatedTiles.push_back({{i, j}, tile->nodes()});
            }
        }
    }
}

//  DEPTH ALGO
QVector<QVector<QVector<bool>>> GameModel::depthAlgo()
{
    int m = _DIM;
    QVector<QVector<QVector<bool>>> v(m, QVector<QVector<bool>>(m, {0, 0, 0, 0}));
    int startTileY = getBounded(0, m), startTileX = getBounded(0, m);
    QStack<QVector<int>> tileStack;
    tileStack.push({startTileY, startTileX});
    this->_startTile = {startTileY, startTileX};
    while (!tileStack.empty()) {
        QVector<int> currentTile = tileStack.top();
        int currentY = currentTile[0], currentX = currentTile[1];
        QVector<QVector<int>> chooseTile {
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
        size_t selectIndex = getBounded(0, chooseTile.size());
        tileStack.push(chooseTile[selectIndex]);
        v[currentY][currentX][chooseTile[selectIndex][2]] = TileModel::ON;
        v[chooseTile[selectIndex][0]][chooseTile[selectIndex][1]][chooseTile[selectIndex][3]] = TileModel::ON;
    }
    return v;
}   // DEPTH ALGO

//  PRIM ALGO
QVector<QVector<QVector<bool>>> GameModel::primAlgo()
{
    int m = _DIM;
    QVector<QVector<QVector<bool>>> v(m, QVector<QVector<bool>>(m, {0, 0, 0, 0}));
    QVector<QVector<bool>> usedV(m, QVector<bool>(m, false));
    int startTileY = getBounded(0, m), startTileX = getBounded(0, m);
    _startTile = {startTileY, startTileX};
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
//  PRIM ALGO
