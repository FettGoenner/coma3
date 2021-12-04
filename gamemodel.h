#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <QObject>
#include <QRandomGenerator>
#include <QTimer>
#include <QTime>

#include "tile.h"

class GameModel : public QObject
{
    Q_OBJECT
public:
    explicit GameModel(size_t row = 7, size_t col = 7, int gameSeed = QRandomGenerator::global()->bounded(-999999999, 999999999), QObject *parent = nullptr);

    bool gameStarted;
    int totalPlayTime;
    int totalSteps;
    size_t dimX;
    size_t dimY;
    QRandomGenerator gen;
    QVector<int> startTile;
    QVector<QVector<Tile*>> game;
    QVector<QVector<QString>> answer;
    QVector<QVector<QString>> resetVector;
    QTimer *timer;

    void setSize(int row, int column);
    void setGameSeed(int seed);
    void clearEverything();
    int getBounded(int lowest, int highest);
    QVector<QVector<QString>> depthAlgo();
    QVector<QVector<QString>> primAlgo();

signals:
    QString sendSteps(QString totalSteps);
    QString sendTime(QString totalTime);
    QString sendGameSeed(QString seed);

public slots:
    void setSteps() {
        if (!gameStarted)
            gameStarted = true;
        ++totalSteps;
        emit sendSteps(QString::number(totalSteps));
    }
    void setTime() {
        int minute = this->totalPlayTime/60;
        int sec = this->totalPlayTime%60;
        QTime time(0, minute, sec);
        emit sendTime(time.toString("mm:ss"));
    }
    bool checkAnswer();
    void resetGame();
    void showSolution();
};

#endif // GAMEMODEL_H
