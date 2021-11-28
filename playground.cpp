#include "playground.h"
#include "linetile.h"
#include "cornertile.h"
#include "endtile.h"
#include "junctiontile.h"
#include <QVector>
#include <QStack>
#include <QDebug>
#include <QPainter>
#include <QPen>
#include <QTime>

PlayGround::PlayGround(QWidget *parent,int playGroundSizeY, int playGroundSizeX, int gameSeed) :
    QWidget(parent)
  , playGroundSizeX(playGroundSizeX) // default is 7 -> 7x7
  , playGroundSizeY(playGroundSizeY) // default is 7 -> 7x7
  , playGround(QVector<QVector<Tile*>>(playGroundSizeY, QVector<Tile*>(playGroundSizeX, nullptr)))
  , resetVector(QVector<QVector<QString>>(playGroundSizeY, QVector<QString>(playGroundSizeX, "")))
  , timer(new QTimer(this))
{
    setGameSeed(gameSeed);
    connect(this->timer, &QTimer::timeout, this, [=]() {
        if (this->gameStarted){
            ++this->totalPlayTime;
            this->setTime();
        }
    });
    this->timer->start(1000);
    // initialize game
    this->initializeGame();
}

void PlayGround::initializeGame()
{
    // get initialized game with QStrings in a 2d Vector
    QVector<QVector<QString>> depthGame = this->depthAlgo();
    // save the correct answer
    this->answer = depthGame;
    this->clearEverything();
    setGameWithVector2d(depthGame);
}

void PlayGround::setSize(int row, int column)
{
    this->playGroundSizeY = row;
    this->playGroundSizeX = column;
    for (int i = 0; i < this->playGround.size(); ++i) {
        for (int j = 0; j < this->playGround[0].size(); ++j) {
            delete this->playGround[i][j];
            this->playGround[i][j] = nullptr;
        }
    }
    this->playGround = QVector<QVector<Tile*>>(playGroundSizeY, QVector<Tile*>(column, nullptr));

    this->resetVector = QVector<QVector<QString>>(playGroundSizeY, QVector<QString>(column, ""));
}

void PlayGround::setGameSeed(int seed)
{
    this->gen.seed(seed);
    emit this->sendGameSeed(QString::number(seed));
}

void PlayGround::setGameWithVector2d(QVector<QVector<QString>> &v)
{

    // initialize different tiles on different positions
    for (int i = 0; i < playGroundSizeY; ++i) {
        for (int j = 0; j < playGroundSizeX; ++j) {
            // get the string of the nodes on the position [i,j]
            QString nodes = v[i][j];

            Tile *tile;
            if (nodes.size() == 1) {
                tile = new EndTile(nodes, Qt::blue);
            } else if (nodes.size() == 3) {
                tile = new JunctionTile(nodes, Qt::blue);
            } else {
                int first = nodes[0].digitValue(), second = nodes[1].digitValue();
                if (first > second) {
                    int temp = first;
                    first = second;
                    second = temp;
                }
                if (second - first == 1 || second - first == 3)
                    tile = new CornerTile(nodes, Qt::blue);

                else
                    tile = new LineTile(nodes, Qt::blue);
            }
            // rotate the tile randomly
            for (int k = 0; k < getBounded(0, 3); ++k)
                tile->rotate90();


            // save tile in 2d vector playGround
            this->playGround[i][j] = tile;
            this->resetVector[i][j] = tile->getNodeString();
            this->gridLayout->addWidget(tile, i, j);

            connect(tile, &Tile::clicked, this, &PlayGround::setSteps);
            connect(tile, &Tile::clicked, this, &PlayGround::checkAnswer);
        }
    }
}

void PlayGround::clearEverything()
{
    if ( this->gridLayout != nullptr )
    {
        QLayoutItem* item;
        while ((item = this->gridLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete this->gridLayout;
        this->gridLayout = nullptr;
    }
    this->gameStarted = false;
    this->totalPlayTime = 0;
    this->totalSteps = 0;
    this->bgc = Qt::white;
    this->update();
    // create gridlayout
    this->gridLayout = new QGridLayout(this);
    this->gridLayout->setSpacing(0);
    setLayout(this->gridLayout);

}

int PlayGround::getBounded(int lowest, int highest)
{
    return gen.bounded(lowest, highest);
}


QVector<QVector<QString>> PlayGround::depthAlgo()
{
    int m = playGroundSizeY, n = playGroundSizeX;
    QVector<QVector<QString>> v(m, QVector<QString>(n, ""));
    int startNodeY = getBounded(0, m), startNodeX = getBounded(0, n);
    QStack<QVector<int>> nodesStack;
    nodesStack.push({startNodeY, startNodeX});
    this->startTile = {startNodeY, startNodeX};
    QVector<int> currentNode;
    while (!nodesStack.empty()) {
        currentNode = nodesStack.top();
        int currentY = currentNode[0], currentX = currentNode[1];
        QVector<QVector<int>> chooseNode{
            {currentY - 1, currentX,     Tile::North},
            {currentY,     currentX + 1, Tile::East},
            {currentY,     currentX - 1, Tile::West},
            {currentY + 1, currentX,     Tile::South},
        };
        for (int i = 0; i < chooseNode.size(); ++i) {
            int y = chooseNode[i][0], x = chooseNode[i][1];
            if (x < 0 || y < 0 || y >= m || x >= n || !v[y][x].isEmpty())
                chooseNode.erase(chooseNode.begin() + (i--));
        }
        if (chooseNode.empty() || v[currentY][currentX].size() == 3) {
            nodesStack.pop();
            continue;
        }
        int selectIndex = getBounded(0, chooseNode.size());
        nodesStack.push(chooseNode[selectIndex]);
        v[currentY][currentX] += QString::number(chooseNode[selectIndex][2]);
        v[chooseNode[selectIndex][0]][chooseNode[selectIndex][1]] +=  QString::number(Tile::getNode(chooseNode[selectIndex][2]));
    }
    return v;
}

void PlayGround::checkAnswer()
{
    int m = playGroundSizeY, n = playGroundSizeX;
    int countTiles = m*n - 1;
    QVector<QVector<QVector<bool>>> checkVecktor(m, QVector<QVector<bool>>(n, {false, false, false, false}));
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j)
            checkVecktor[i][j] = playGround[i][j]->nodes;

    QStack<QVector<int>> nodesStack;
    nodesStack.push(this->startTile);
    QVector<int> currentNode;
    while (!nodesStack.empty()) {

        currentNode = nodesStack.top();
        int currentY = currentNode[0], currentX = currentNode[1];
        QVector<QVector<int>> chooseNode{
            {currentY - 1, currentX,     Tile::North},
            {currentY,     currentX + 1, Tile::East},
            {currentY + 1, currentX,     Tile::South},
            {currentY,     currentX - 1, Tile::West},
        };
        for (int i = 0, j = 0; i < 4; ++i, ++j) {
            if (!checkVecktor[currentY][currentX][i]) {
                chooseNode.erase(chooseNode.begin() + j);
                --j;
            }
        }

        if (chooseNode.empty()) {
            nodesStack.pop();
            continue;
        }

        for (int i = 0; i < chooseNode.size(); ++i) {
            int y = chooseNode[i][0], x = chooseNode[i][1];
            if (x < 0 || y < 0 || y >= m || x >= n || Tile::noNodes(checkVecktor[y][x])) {
                this->bgc = Qt::white;
                update();
                return;
            }
        }
        --countTiles;
        nodesStack.push(chooseNode[0]);
        checkVecktor[chooseNode[0][0]][chooseNode[0][1]][Tile::getNode(chooseNode[0][2])] = false;
        checkVecktor[currentY][currentX][chooseNode[0][2]] = false;

    }
    if (countTiles == 0){
        this->bgc = Qt::green;
        gameStarted = false;
    }
    else
        this->bgc = Qt::white;
    update();
}

void PlayGround::resetGame()
{

    this->gameStarted = false;
    this->totalPlayTime = 0;
    this->totalSteps = 0;
    this->totalPlayTime = 0;
    this->bgc = Qt::white;
    this->update();
    for (int i = 0; i < this->playGroundSizeY; ++i) {
        for (int j = 0; j < this->playGroundSizeX; ++j) {
            this->playGround[i][j]->setNodes(this->resetVector[i][j]);
        }
    }

}


void PlayGround::paintEvent(QPaintEvent *ev)
{
    int width = this->width(), height = this->height();
    // draw the deflaut StraightNode, from top to bottom
    QPainter painter(this);

    QPen pen;
    pen.setColor(QColor(255, 0, 0, 0));

    painter.setPen(pen);
    QBrush brush;
    brush.setColor(this->bgc);
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);
    painter.drawRect(0, 0, width, height);

    return QWidget::paintEvent(ev);
}
