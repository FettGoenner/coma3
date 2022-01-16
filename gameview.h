#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QWidget>
#include <QGridLayout>
#include <QTimer>
#include <QTime>
#include <QPainter>

#include "gamemodel.h"
#include "tilecontroler.h"

class GameView : public QWidget
{
    Q_OBJECT
private:
    GameModel *gameModel;
    QGridLayout *gridLayout = nullptr;

    void clearLayout();
public:
    explicit GameView(GameModel *gameModel, QWidget *parent = nullptr);

signals:

public slots:
    void showGame();
protected:
    void paintEvent(QPaintEvent*) override;
};

#endif // GAMEVIEW_H
