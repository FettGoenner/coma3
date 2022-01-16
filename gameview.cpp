#include "gameview.h"

GameView::GameView(GameModel *gameModel, QWidget *parent) :
    QWidget(parent)
  , _model(gameModel)
{
    // initialize game
    this->showGame();
    connect(this->_model, &GameModel::onGameInit, this, &GameView::showGame);
}

void GameView::showGame()
{
    this->setLayout(this->_model->getLayout());
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


