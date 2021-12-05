#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <QObject>
#include <QRandomGenerator>
#include <QTimer>
#include <QTime>

#include "tilemodel.h"

class GameModel : public QObject
{
    Q_OBJECT

public:
    enum GameType{Depth = 0, Prim};

    explicit GameModel(size_t row = 7, size_t col = 7, int gameSeed = QRandomGenerator::global()->bounded(-999999999, 999999999), QObject *parent = nullptr);

    bool gameStarted = false;
    int totalPlayTime;
    int totalSteps;
    size_t dimX;
    size_t dimY;
    int algoType = GameModel::Depth;
    QRandomGenerator gen; // for random values
    QVector<int> startTile;
    QVector<QVector<TileModel*>> game;
    QVector<QVector<QString>> answer;
    QVector<QVector<QString>> resetVector;
    QTimer *timer;

    void initializeGame(int algo = GameModel::Depth);
    void setSize(int row, int column);
    void setGameSeed(int seed);
    void clearEverything();
    int getBounded(int lowest, int highest);
    size_t getRow() const;
    size_t getCol() const;
    QVector<QVector<QString>> depthAlgo();
    QVector<QVector<QString>> primAlgo();
    QString getAlgoType();

signals:
    void onGameInitialization(bool clearStatus = false);
    QString sendSteps(QString totalSteps);
    QString sendTime(QString totalTime);
    QString sendGameSeed(QString seed);
    bool onGameStatus(bool status); // true, if this is a solution
    void gameStart();
public slots:
    void setSteps() {
        if (!this->gameStarted){
            this->gameStarted = true;
            emit this->gameStart();
        }
        ++totalSteps;
        emit sendSteps(QString::number(totalSteps));
    }
    void setTime();
    bool checkAnswer();
    void resetGame();
    void showSolution();
};

#endif // GAMEMODEL_H
