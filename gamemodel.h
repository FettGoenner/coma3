#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <QObject>
#include <QRandomGenerator>
#include <QTimer>
#include <QTime>
#include <QStack>
#include <QGridLayout>

#include "tilemodel.h"
#include "endtilemodel.h"
#include "junctiontilemodel.h"
#include "cornertilemodel.h"
#include "linetilemodel.h"
#include "tileview.h"
#include "tilecontroler.h"

class GameModel : public QObject
{
    Q_OBJECT
private:
    size_t m_dim;
    size_t m_totalPlayTime;
    size_t m_totalSteps;
    size_t m_algoType = GameModel::Depth;
    size_t m_hintCount = 0;
    size_t m_gameSeed;
    QRandomGenerator m_gen; // for random values
    QVector<int> m_startTile;
    QVector<QVector<TileModel*>> m_game;
    QVector<QVector<QVector<bool>>> m_answer;
    QVector<QVector<QVector<bool>>> m_resetVector;
    QTimer *m_timer;
    QVector<QPair<size_t, size_t>> m_hintedTiles;
    QVector<QPair<QPair<size_t, size_t>, QVector<bool>>> m_rotatedTiles;
    QGridLayout *m_gridLayout = nullptr;

    void clearEverything(bool clearLayout = false);
    QVector<QVector<QVector<bool>>> depthAlgo();
    QVector<QVector<QVector<bool>>> primAlgo();
    void setTotalTime(size_t time);
    void setStep(size_t steps);
    void clearLayout();

public:
    enum GameType{Depth = 0, Prim};
    static const size_t HINT_LIMIT = 3; // Hint limit
    static const size_t MAX_SIZE = 30;
    static const size_t MIN_SIZE = 2;
    explicit GameModel(size_t size = 7, size_t gameSeed = QRandomGenerator::global()->bounded(0, INT_MAX), QObject *parent = nullptr);

    bool gameStarted = false;


    // set-method
    void initializeGame(GameModel::GameType algo = GameModel::Depth);
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
    QGridLayout* getLayout() const;

signals:
    void onGameInitialization();
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
        ++m_totalSteps;
        emit sendSteps(QString::number(m_totalSteps));
    }
    void setTime();
    bool checkAnswer();
    void resetGame();
    void showSolution();

    void showSolutionOnRandomTile();
    void changeGameStarted(bool started);

};

#endif // GAMEMODEL_H
