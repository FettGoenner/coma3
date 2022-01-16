#include "gamemodel.h"

GameModel::GameModel(size_t size, size_t gameSeed, QObject *parent) :
    QObject(parent)
  , m_dim(size)
  , m_resetVector(QVector<QVector<QVector<bool>>>(m_dim, QVector<QVector<bool>>(m_dim, {})))
  , m_timer(new QTimer)
  , game(QVector<QVector<TileModel*>>(m_dim, QVector<TileModel*>(m_dim, nullptr)))

{
    setGameSeed(gameSeed);
    connect(this->m_timer, &QTimer::timeout, this, &GameModel::setTime);
    this->m_timer->start(1000);

    this->initializeGame();
}

void GameModel::initializeGame(GameModel::GameType algo)
{
    this->m_algoType = algo;
    // get initialized game with QStrings in a 2d Vector
    QVector<QVector<QVector<bool>>> gameAfterAlgo;
    if (algo == GameModel::Depth)
        gameAfterAlgo = this->depthAlgo();
    else if (algo == GameModel::Prim)
        gameAfterAlgo = this->primAlgo();
    else
        throw "Unknown algo type";

    // save the correct answer
    this->m_answer = gameAfterAlgo;
    this->clearEverything(true);
    // initialize different tiles on different positions
    for (size_t i = 0; i < this->m_dim; ++i) {
        for (size_t j = 0; j < this->m_dim; ++j) {
            // get the string of the nodes on the position [i,j]
            QVector<bool> nodes = gameAfterAlgo[i][j];
            TileModel *tileModel;
            int tileType = TileModel::getTileTypeByVector(nodes);
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

            // rotate the tile randomly
            for (int k = 0; k < getBounded(0, 4); ++k)
                tileModel->rotate90();

            // save tile in 2d vector playGround
            this->game[i][j] = tileModel;
            this->m_resetVector[i][j] = tileModel->getNodeVector();

            // create TileView
            TileView *tileView = new TileView(tileModel, Qt::blue);

            this->m_gridLayout->addWidget(tileView, i, j);

            TileControler *tileControler = new TileControler(tileModel, tileView, this);

            // set steps after clicked a tile
            connect(tileControler, &TileControler::clicked, this, &GameModel::setSteps);

            // check answer after clicked a tile
            connect(tileControler, &TileControler::clicked, this, &GameModel::checkAnswer);

            // set tile's color between green and blue
            connect(this, &GameModel::onGameStatus, tileView, &TileView::isConnected);

            connect(tileModel, &TileModel::nodesChangedByView, this, &GameModel::tileRotatedByView);
        }
    }
    emit this->onGameInitialization();
    emit this->hintSuccessed(GameModel::HINT_LIMIT);
}

void GameModel::loadGame(const size_t dim, const size_t seed, const QString &gameAlgo, const size_t totalPlayTime, const size_t totalSteps, const size_t hintRamaining, const QVector<QPair<size_t, size_t>> &hintedTiles, const QVector<QPair<QPair<size_t, size_t>, QVector<bool>>> &rotatedTiles)
{
    this->setSize(dim);

    this->setGameSeed(seed);

    GameModel::GameType algoType = gameAlgo == "Depth" ? GameModel::Depth : GameModel::Prim;

    this->initializeGame(algoType);

    this->setTotalTime(totalPlayTime);

    this->setStep(totalSteps);

    this->m_hintCount = GameModel::HINT_LIMIT - hintRamaining;
    emit this->hintSuccessed(hintRamaining);

    for (const auto &pos : hintedTiles) {
        this->game[pos.first][pos.second]->setNodes(this->m_answer[pos.first][pos.second]);
        emit this->game[pos.first][pos.second]->rotatedByHint();
    }
    this->m_hintedTiles = hintedTiles;

    for (const auto &pair : rotatedTiles) {
        QPair<size_t, size_t> pos = pair.first;
        QVector<bool> nodes = pair.second;

        this->game[pos.first][pos.second]->setNodes(nodes);
    }
    this->m_rotatedTiles = rotatedTiles;
    this->checkAnswer();
}

void GameModel::setSize(size_t size)
{
    this->m_dim = size;
    for (int i = 0; i < this->game.size(); ++i) {
        for (int j = 0; j < this->game[0].size(); ++j) {
            delete this->game[i][j];
            this->game[i][j] = nullptr;
        }
    }
    this->game = QVector<QVector<TileModel*>>(this->m_dim, QVector<TileModel*>(this->m_dim, nullptr));

    this->m_resetVector = QVector<QVector<QVector<bool>>>(this->m_dim, QVector<QVector<bool>>(this->m_dim, {}));
}

void GameModel::setGameSeed(size_t seed)
{
    this->m_gameSeed = seed;
    this->m_gen.seed(seed);
    emit this->sendGameSeed(QString::number(seed));
}

void GameModel::clearEverything(bool clearLayout)
{
    this->gameStarted = false;
    this->m_totalPlayTime = 0;
    this->m_totalSteps = 0;
    this->m_hintCount = 0;
    this->m_hintedTiles = QVector<QPair<size_t, size_t>>();
    this->m_rotatedTiles = QVector<QPair<QPair<size_t, size_t>, QVector<bool>>>();
    if (clearLayout)
        this->clearLayout();
}

int GameModel::getBounded(int lowest, int highest)
{
    return this->m_gen.bounded(lowest, highest);
}

size_t GameModel::getSize() const
{
    return this->m_dim;
}

size_t GameModel::getGameSeed() const
{
    return this->m_gameSeed;
}

QString GameModel::getAlgoType() const
{
    if (this->m_algoType == GameModel::Depth)
        return "Depth";
    else if (this->m_algoType == GameModel::Prim)
        return "Prim";
    return "";
}

size_t GameModel::getTotalPlayTime() const
{
    return this->m_totalPlayTime;
}

size_t GameModel::getTotalSteps() const
{
    return this->m_totalSteps;
}

size_t GameModel::getHintRamaining() const
{
    return GameModel::HINT_LIMIT - this->m_hintCount;
}

QVector<QPair<size_t, size_t> > GameModel::getHintedTiles() const
{
    return this->m_hintedTiles;
}

QVector<QPair<QPair<size_t, size_t>, QVector<bool> > > GameModel::getRotatedTiles() const
{
    return this->m_rotatedTiles;
}

QGridLayout *GameModel::getLayout() const
{
    return this->m_gridLayout;
}

void GameModel::tileRotatedByView(TileModel *tile)
{
    for (size_t i = 0; i < this->m_dim; ++i) {
        for (size_t j = 0; j < this->m_dim; ++j) {
            if (this->game[i][j] == tile) {
                auto it = this->m_rotatedTiles.begin();
                while (it != this->m_rotatedTiles.end()) { // check if tile's position already in the vector
                    QPair<size_t, size_t> pos = it->first;
                    if (pos.first == i && pos.second == j) { // update the nodes of the tile
                        it->second = tile->getNodeVector();
                        break;
                    }
                    ++it;
                }
                if (it == this->m_rotatedTiles.end()) // if the tile not exsist in vector rotatedTiles
                    this->m_rotatedTiles.push_back({{i, j}, tile->getNodeVector()});
            }
        }
    }
}

QVector<QVector<QVector<bool>>> GameModel::depthAlgo()
{
    int m = this->m_dim;
    QVector<QVector<QVector<bool>>> v(m, QVector<QVector<bool>>(m, {0, 0, 0, 0}));
    int startTileY = getBounded(0, m), startTileX = getBounded(0, m);
    QStack<QVector<int>> tileStack;
    tileStack.push({startTileY, startTileX});
    this->m_startTile = {startTileY, startTileX};
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
        size_t selectIndex = getBounded(0, chooseTile.size());
        tileStack.push(chooseTile[selectIndex]);
        v[currentY][currentX][chooseTile[selectIndex][2]] = TileModel::ON;
        v[chooseTile[selectIndex][0]][chooseTile[selectIndex][1]][chooseTile[selectIndex][3]] = TileModel::ON;
    }
    return v;
}

QVector<QVector<QVector<bool>>> GameModel::primAlgo()
{
    int m = this->m_dim;
    QVector<QVector<QVector<bool>>> v(m, QVector<QVector<bool>>(m, {0, 0, 0, 0}));
    QVector<QVector<bool>> usedV(m, QVector<bool>(m, false));
    int startTileY = getBounded(0, m), startTileX = getBounded(0, m);
    this->m_startTile = {startTileY, startTileX};
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

void GameModel::setTotalTime(size_t totalTime)
{
    this->m_totalPlayTime = totalTime;
    int minute = this->m_totalPlayTime/60;
    int sec = this->m_totalPlayTime%60;
    QTime time(0, minute, sec);
    emit sendTime(time.toString("mm:ss"));
}

void GameModel::setStep(size_t steps)
{
    this->m_totalSteps = steps;
    emit sendSteps(QString::number(m_totalSteps));
}

void GameModel::clearLayout()
{
    if (this->m_gridLayout != nullptr)
    {
        QLayoutItem* item;
        while ((item = this->m_gridLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete this->m_gridLayout;
        this->m_gridLayout = nullptr;
    }

    // create gridlayout
    this->m_gridLayout = new QGridLayout;
    this->m_gridLayout->setSpacing(0);
    this->m_gridLayout->setContentsMargins(0, 0, 0, 0);
}

void GameModel::setTime()
{
    if (this->gameStarted) {
        ++this->m_totalPlayTime;
        int minute = this->m_totalPlayTime/60;
        int sec = this->m_totalPlayTime%60;
        QTime time(0, minute, sec);
        emit sendTime(time.toString("mm:ss"));
    }
}

bool GameModel::checkAnswer()
{
    int m = this->m_dim;
    size_t countTiles = m*m - 1;
    QVector<QVector<QVector<bool>>> checkVecktor(m, QVector<QVector<bool>>(m, {false, false, false, false}));
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < m; ++j)
            checkVecktor[i][j] = this->game[i][j]->getNodeVector();

    QStack<QVector<int>> tilesStack;
    tilesStack.push(this->m_startTile);
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
    if (countTiles == 0) {
        this->gameStarted = false;
        emit onGameStatus(true);
        return true;
    }
    else {
        emit onGameStatus(false);
        return false;
    }
}

void GameModel::resetGame()
{
    this->clearEverything();
    for (size_t i = 0; i < this->m_dim; ++i) {
        for (size_t j = 0; j < this->m_dim; ++j) {
            emit this->game[i][j]->resetedTile();
            this->game[i][j]->setNodes(this->m_resetVector[i][j]);
        }
    }
    emit onGameStatus(false);
    emit this->hintSuccessed(GameModel::HINT_LIMIT);
}

void GameModel::showSolution()
{
    this->gameStarted = false;
    for (size_t i = 0; i < this->m_dim; ++i) {
        for (size_t j = 0; j < this->m_dim; ++j) {
            emit this->game[i][j]->resetedTile();
            this->game[i][j]->setNodes(this->m_answer[i][j]);
        }
    }
    emit onGameStatus(true);
}

void GameModel::showSolutionOnRandomTile()
{
    if (this->m_hintCount >= GameModel::HINT_LIMIT)
        return;

    if (!this->gameStarted) {
        this->gameStarted = true;
        emit this->gameStart();
    }
    QVector<QPair<size_t, size_t>> unSolvedTiles;

    // set all unsolved tiles in a vector
    for (size_t i = 0; i < this->m_dim; ++i)
        for (size_t j = 0; j < this->m_dim; ++j)
            if (this->game[i][j]->getNodeVector() != this->m_answer[i][j])
                unSolvedTiles.push_back({i, j});
    // get the position randomly
    QPair<size_t, size_t> position = unSolvedTiles[this->getBounded(0, unSolvedTiles.size())];
    this->game[position.first][position.second]->setNodes(this->m_answer[position.first][position.second]);

    this->m_hintedTiles.push_back(position);
    emit this->game[position.first][position.second]->rotatedByHint();
    emit this->hintSuccessed(GameModel::HINT_LIMIT - ++this->m_hintCount);
    this->checkAnswer();
}

void GameModel::changeGameStarted(bool started)
{
    this->gameStarted = started;
    if (this->gameStarted == false)
        emit this->gamePaused();
}

