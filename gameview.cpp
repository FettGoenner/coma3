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
    connect(this->gameModel, &GameModel::onGameStatus, this, &GameView::changeBgc);
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

            connect(tileView, &TileView::clicked, this->gameModel, &GameModel::setSteps);
            connect(tileView, &TileView::clicked, this->gameModel, &GameModel::checkAnswer);
            connect(tileView, &TileView::clickedWhileHint, this->gameModel, &GameModel::showSolutionOnTile);
            connect(tileView, &TileView::clickedWhileHint, this, &GameView::endHint);
            connect(this, &GameView::stopTileHintAnimation, tileView, &TileView::stopHintAnimation);
            connect(this, &GameView::startTileHintAnimation, tileView, &TileView::startHintAnimation);
        }
    }
}

void GameView::clearLayout()
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
    this->bgc = Qt::white;
    this->update();
    // create gridlayout
    this->gridLayout = new QGridLayout(this);
    this->gridLayout->setSpacing(0);
    this->gridLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(this->gridLayout);

}

void GameView::changeBgc(bool connected)
{
    if (connected)
        this->bgc = Qt::green;
    else
        this->bgc = Qt::white;
    update();
}

void GameView::startHint()
{
    if (!this->hintStarted) {
        this->hintStarted = true;
        emit this->startTileHintAnimation();
    } else
        this->endHint();
}

void GameView::endHint()
{
    this->hintStarted = false;
    emit this->stopTileHintAnimation();
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


