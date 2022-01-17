#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <QObject>
#include <QRandomGenerator>
#include <QTimer>
#include <QTime>
#include <QGridLayout>
#include "tilemodel.h"


class GameModel : public QObject
{
    Q_OBJECT
public:
    static const size_t MAX_SIZE = 30;
    static const size_t MIN_SIZE = 2;
    static const size_t HINTLIMIT = 3;

    enum GameType{Depth = 0, Prim};

private:
    size_t _DIM;
    size_t _totalTime;
    size_t _totalSteps;
    size_t _gameSeed;
    GameModel::GameType _algo = GameModel::Depth;
    size_t _hintCount = 0;
    QRandomGenerator gen; // for random values
    QVector<int> _startTile;
    QVector<QVector<QVector<bool>>> _answer; // 2D array of tiles
    QVector<QVector<QVector<bool>>> _resetVector;    //  2D array of tiles
    QTimer *_timer;
    QGridLayout *gridLayout;
    QVector<QPair<size_t, size_t>> _hintedTiles;
    QVector<QPair<QPair<size_t, size_t>, QVector<bool>>> _rotatedTiles;



    void clearEverything(bool clearLayout = false);
    QVector<QVector<QVector<bool>>> depthAlgo();
    QVector<QVector<QVector<bool>>> primAlgo();

    // following where public
    bool _started = false;
    QVector<QVector<TileModel*>> _game;

    void clear();

public:
    explicit GameModel(size_t size = 7, size_t gameSeed = QRandomGenerator::global()->bounded(0, INT_MAX), QObject *parent = nullptr);

    void loadGame(const size_t, const size_t, const QString&, const size_t, const size_t, const size_t, const QVector<QPair<size_t, size_t>>&, const QVector<QPair<QPair<size_t, size_t>, QVector<bool>>>&);
    //  GETERS
    QGridLayout* getLayout() const { return gridLayout; }
    size_t getHintRamaining() const{return GameModel::HINTLIMIT - this->_hintCount;}
    QVector<QPair<QPair<size_t, size_t>, QVector<bool>>> getRotatedTiles() const{return this->_rotatedTiles;}
    QVector<QPair<size_t, size_t> > getHintedTiles() const{return this->_hintedTiles;}
    size_t getTotalPlayTime() const {return _totalTime; }
    bool started() const { return _started; }
    size_t getTotalSteps() const { return _totalSteps; }
    size_t getSeed() const {return _gameSeed; }
    QVector<QVector<TileModel*>>& game() { return _game; }
    //  SETERS
    void setStep(size_t steps)
    {
        _totalSteps = steps;
        emit sendSteps(QString::number(_totalSteps));
    }
    void initGame(GameModel::GameType algo  =Depth);
    void setSize(size_t size);
    void setSeed(size_t seed)
    {
        _gameSeed = seed;
        gen.seed(seed);
        emit sendSeed(QString::number(seed));
    }
    void setTotalTime(size_t totTime){
        _totalTime = totTime;
        int minute = _totalTime/60;
        int sec = _totalTime%60;
        QTime time(0, minute, sec);
        emit sendTime(time.toString("mm:ss"));
    }

    int getBounded(int lowest, int highest){ return gen.bounded(lowest, highest); }
    size_t getSize() const{ return _DIM; }
    QString getAlgo();

signals:
    void onGameInit(bool clearStatus = false);
    QString sendSteps(QString totalSteps);
    QString sendTime(QString totalTime);
    QString sendSeed(QString seed);
    bool onGameStatus(bool status); // true, if this is a solution
    void gameStart();
    void initializedNewGame();
    void paused();
    void hintSuccessed(int remaining);

private slots:
    void tileRotatedByView(TileModel *tile);

public slots:
    void setSteps() {
        if (!_started) {
            _started = true;
            emit gameStart();
        }
        ++_totalSteps;
        emit sendSteps(QString::number(_totalSteps));
    }
    void setTime();
    bool checkAnswer();
    void resetGame();
    void showSolution();

    void showSolutionOnRandomTile();
    void changeGameStarted(bool started);

};

#endif // GAMEMODEL_H
