#include "gameview.h"

GameView::GameView(GameModel *gameModel, QWidget *parent) :
    QWidget(parent)
  , _model(gameModel)
{
    // initialize game
    showGame();
    connect(_model, &GameModel::onGameInit, this, &GameView::showGame);
}

void GameView::showGame()
{
    setLayout(_model->getLayout());
}


void GameView::paintEvent(QPaintEvent *ev)
{
    size_t w = width(), h = height();
    // draw the deflaut StraightNode, from top to bottom
    QPainter painter(this);

    painter.setPen(Qt::NoPen);
    QBrush brush;
    brush.setColor(Qt::white);
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);
    painter.drawRect(0, 0, w, h);

    // keep aspect ratio 1:1 (square)
    size_t size = std::min(w, h);
    resize(size, size);

    // keep the playground at the mid
    if (w > h)
        move((w - h)/2, 0);
    else if (w < h)
        move(0, (h - w)/2);

    return QWidget::paintEvent(ev);
}


