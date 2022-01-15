#include <QVector>
#include <QStack>
#include <QPainter>
#include <QTime>
#include <QObject>

#include "gameview.h"

GameView::GameView(GameModel *gameModel, QWidget *parent) :
    QWidget(parent)
  , gameModel(gameModel)
{
    // initialize game
    this->showGame(true);
    connect(this->gameModel, &GameModel::onGameInitialization, this, &GameView::showGame);
}

void GameView::showGame(bool clearStatus)
{
    if (clearStatus)
        this->clearLayout();
    // initialize different tiles on different positions
    for (size_t i = 0; i < this->gameModel->getSize(); ++i) {
        for (size_t j = 0; j < this->gameModel->getSize(); ++j) {
            TileView * tileView = new TileView(this->gameModel->game[i][j], Qt::blue);
            // save tile in 2d vector playGround
            this->gridLayout->addWidget(tileView, i, j);

            // set steps after clicked a tile
            connect(tileView, &TileView::clicked, this->gameModel, &GameModel::setSteps);

            // check answer after clicked a tile
            connect(tileView, &TileView::clicked, this->gameModel, &GameModel::checkAnswer);

            // set tile's color between green and blue
            connect(gameModel, &GameModel::onGameStatus, tileView, &TileView::isConnected);

        }
    }

}


void GameView::clearLayout()
{
    if (this->gridLayout != nullptr)
    {
        QLayoutItem* item;
        while ((item = this->gridLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete this->gridLayout;
        this->gridLayout = nullptr;
    }
    this->update();
    // create gridlayout
    this->gridLayout = new QGridLayout(this);
    this->gridLayout->setSpacing(0);
    this->gridLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(this->gridLayout);
}


void GameView::paintEvent(QPaintEvent *ev)
{
    size_t width = this->width(), height = this->height();
    // draw the deflaut StraightNode, from top to bottom
    QPainter painter(this);

    painter.setPen(Qt::NoPen);
    QBrush brush;
    brush.setColor(Qt::white);
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);
    painter.drawRect(0, 0, width, height);

    // keep aspect ratio 1:1 (square)
    size_t size = std::min(width, height);
    this->resize(size, size);

    // keep the playground at the mid
    if (width > height)
        this->move((width - height)/2, 0);
    else if (width < height)
        this->move(0, (height - width)/2);

    return QWidget::paintEvent(ev);
}


