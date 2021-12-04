#include <QVector>
#include <QStack>
#include <QPainter>
#include <QTime>

#include "gameview.h"
#include "linetile.h"
#include "cornertile.h"
#include "endtile.h"
#include "junctiontile.h"
GameView::GameView(GameModel *gameModel, QWidget *parent) :
    QWidget(parent)
  , gameModel(gameModel)
{
    // initialize game
    this->initializeGame();
}

void GameView::initializeGame(QString algo)
{
    // get initialized game with QStrings in a 2d Vector
    QVector<QVector<QString>> game;
    if (algo == "Depth")
        game = this->gameModel->depthAlgo();
    else if (algo == "Prim")
        game = this->gameModel->primAlgo();
    // save the correct answer
    this->gameModel->answer = game;
    this->gameModel->clearEverything();
    setGameWithVector2d(game);
}

void GameView::setGameWithVector2d(QVector<QVector<QString>> &v)
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
            //            for (int k = 0; k < getBounded(0, 3); ++k)
            //                tile->rotate90();
            tile->rotateWithAnimation(getBounded(0, 4) * 90);

            // save tile in 2d vector playGround
            this->playGround[i][j] = tile;
            this->resetVector[i][j] = tile->getNodeString();
            this->gridLayout->addWidget(tile, i, j);

            connect(tile, &Tile::clicked, this, &GameView::setSteps);
            connect(tile, &Tile::clicked, this, &GameView::checkAnswer);
        }
    }
}

void GameView::paintEvent(QPaintEvent *ev)
{
    int width = this->width(), height = this->height();
    // draw the deflaut StraightNode, from top to bottom
    QPainter painter(this);

    painter.setPen(Qt::NoPen);
    QBrush brush;
    brush.setColor(this->bgc);
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);
    painter.drawRect(0, 0, width, height);

    return QWidget::paintEvent(ev);
}
