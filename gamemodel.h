#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <QObject>
#include <QRandomGenerator>
#include <QTimer>
#include <QTime>
#include <QStack>

#include "tilemodel.h"
#include "endtilemodel.h"
#include "junctiontilemodel.h"
#include "cornertilemodel.h"
#include "linetilemodel.h"


class GameModel : public QObject
{
    Q_OBJECT
private:
    size_t _DIM;
    size_t totalPlayTime;
    size_t totalSteps;
    size_t algoType = GameModel::Depth;
    size_t hintCount = 0;
    size_t gameSeed;
    QRandomGenerator gen; // for random values
    QVector<int> startTile;
    QVector<QVector<QVector<bool>>> answer;
    QVector<QVector<QVector<bool>>> resetVector;
    QTimer *timer;
    QVector<QPair<size_t, size_t>> hintedTiles;
    QVector<QPair<QPair<size_t, size_t>, QVector<bool>>> rotatedTiles;

    void clearEverything();
    QVector<QVector<QVector<bool>>> depthAlgo();
    QVector<QVector<QVector<bool>>> primAlgo();
    void setTotalTime(size_t time);
    void setStep(size_t steps);

public:
    enum GameType{Depth = 0, Prim};
    static const size_t HINT_LIMIT = 3; // Hint limit
    static const size_t MAX_SIZE = 30;
    static const size_t MIN_SIZE = 2;
    explicit GameModel(size_t size = 7, size_t gameSeed = QRandomGenerator::global()->bounded(0, INT_MAX), QObject *parent = nullptr);

    bool gameStarted = false;
    QVector<QVector<TileModel*>> game;

    // set-method
    void initializeGame(int algo = GameModel::Depth);
    void loadGame(const size_t dim, const size_t seed, const QString &gameAlgo, const size_t totalPlayTime, const size_t totalSteps, const size_t hintRamaining, const QVector<QPair<size_t, size_t>> &hintedTiles, const QVector<QPair<QPair<size_t, size_t>, QVector<bool>>> &rotatedTiles);
    void setSize(size_t size);
    void setGameSeed(size_t seed);

    // get-method
    int getBounded(int lowest, int highest);
    size_t getSize() const;
    size_t getGameSeed() const;
    QString getAlgoType() const;
    size_t getTotalPlayTime() const;
    size_t getTotalSteps() const;
    size_t getHintRamaining() const;
    QVector<QPair<size_t, size_t>> getHintedTiles() const;
    QVector<QPair<QPair<size_t, size_t>, QVector<bool>>> getRotatedTiles() const;


signals:
    void onGameInitialization(bool clearStatus = false);
    QString sendSteps(QString totalSteps);
    QString sendTime(QString totalTime);
    QString sendGameSeed(QString seed);
    bool onGameStatus(bool status); // true, if this is a solution
    void gameStart();
    void initializedNewGame();
    void gamePaused();
    void hintSuccessed(int remaining);

private slots:
    void tileRotatedByView(TileModel *tile);
public slots:
    void setSteps() {
        if (!this->gameStarted) {
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

    void showSolutionOnRandomTile();
    void changeGameStarted(bool started);

};

#endif // GAMEMODEL_H
