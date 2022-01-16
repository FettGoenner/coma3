#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QWidget>
#include <QVector>

#include <QRandomGenerator>
#include <QTimer>
#include <QTime>
#include <QPainter>
#include "tilemodel.h"
#include "gamemodel.h"
#include "tileview.h"
class GameView : public QWidget
{
    Q_OBJECT

    GameModel *_model;

public:
    explicit GameView(GameModel*, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent*) override;

signals:

public slots:
    void showGame();

};

#endif // GAMEVIEW_H
