#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QWidget>
#include <QVector>
#include <QGridLayout>
#include <QRandomGenerator>
#include <QTimer>
#include <QTime>

#include "tile.h"
#include "gamemodel.h"

class GameView : public QWidget
{
    Q_OBJECT
private:
    GameModel *gameModel;
    QGridLayout *gridLayout = nullptr;
public:
    explicit GameView(GameModel *gameModel, QWidget *parent = nullptr);

    void initializeGame(QString algo = "Depth");

    void setGameWithVector2d(QVector<QVector<QString>> &v);

protected:
    void paintEvent(QPaintEvent*) override;
};

#endif // GAMEVIEW_H
