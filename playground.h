#ifndef PLAYGROUND_H
#define PLAYGROUND_H

#include <QWidget>
#include <QVector>
#include <QGridLayout>
#include <QRandomGenerator>
#include <QTimer>
#include <QTime>

#include "tile.h"

class PlayGround : public QWidget
{
    Q_OBJECT
public:
    explicit PlayGround(QWidget *parent = nullptr, int playGroundSizeX = 7, int playGroundSizeY = 7, int gameSeed = QRandomGenerator::global()->bounded(-999999999, 999999999));
    bool gameStarted;
    int totalPlayTime;
    int totalSteps;
    int playGroundSizeX;
    int playGroundSizeY;
    QGridLayout *gridLayout = nullptr;
    QRandomGenerator gen;
    QColor bgc = Qt::white;
    QVector<int> startTile;
    QVector<QVector<Tile*>> playGround;
    QVector<QVector<QString>> answer;
    QVector<QVector<QString>> resetVector;
    QTimer *timer;

    void initializeGame(QString algo = "Depth");
    void setSize(int row, int column);
    void setGameSeed(int seed);
    void setGameWithVector2d(QVector<QVector<QString>> &v);
    void clearEverything();
    int getBounded(int lowest, int highest);
    QVector<QVector<QString>> depthAlgo();
    QVector<QVector<QString>> primAlgo();

protected:
    void paintEvent(QPaintEvent*) override;
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
    void checkAnswer();
    void resetGame();
    void showSolution();
};

#endif // PLAYGROUND_H
