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
private:
    size_t _DIM;
    int totalPlayTime;
    int totalSteps;
    int algoType = GameModel::Depth;
    QRandomGenerator gen; // for random values
    QVector<int> startTile;
    QVector<QVector<QString>> answer;
    QVector<QVector<QString>> resetVector;
    QTimer *timer;

    void clearEverything();
    QVector<QVector<QString>> depthAlgo();
    QVector<QVector<QString>> primAlgo();

public:
    enum GameType{Depth = 0, Prim};

    explicit GameModel(size_t size = 7, size_t gameSeed = QRandomGenerator::global()->bounded(0, INT_MAX), QObject *parent = nullptr);

    bool gameStarted = false;
    QVector<QVector<TileModel*>> game;
    void initializeGame(int algo = GameModel::Depth);
    void setSize(size_t size);
    void setGameSeed(size_t seed);

    int getBounded(int lowest, int highest);
    size_t getSize() const;

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
    void startHint();
    void endHint();
    void showSolutionOnTile(TileModel *tileModel);
};

#endif // GAMEMODEL_H
