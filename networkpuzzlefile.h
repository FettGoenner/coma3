#ifndef NETWORKPUZZLEFILE_H
#define NETWORKPUZZLEFILE_H

#include <QFile>
#include <QObject>
#include <QVector>
#include <QPair>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "gamemodel.h"

class NetworkPuzzleFile : public QObject
{
    Q_OBJECT
private:
    QFile gameFile;
    QString fileName;
    size_t dim;
    size_t seed;
    QString gameAlgo;
    size_t totalPlayTime;
    size_t totalSteps;
    size_t hintRemaining;
    bool dataValid;
    QVector<QPair<size_t, size_t>> hintedTiles; // QPair<size_t, size_t> saves the position of the tiles
    QVector<QPair<QPair<size_t, size_t>, QVector<bool>>> rotatedTiles; // QVector<int> saves the nodes of the tiles

public:
    NetworkPuzzleFile();
    explicit NetworkPuzzleFile(QObject *parent = nullptr);
    NetworkPuzzleFile(const QString &fileName, QObject *parent = nullptr);
    void loadData();
    void writeData(GameModel *gameModel);
    bool isValid();

    size_t getDim() const;
    size_t getSeed() const;
    QString getGameAlgo() const;
    size_t getTotalPlayTime() const;
    size_t getTotalSteps() const;
    size_t getHintRemaining() const;
    QVector<QPair<size_t, size_t>> getHintedTiles() const;
    QVector<QPair<QPair<size_t, size_t>, QVector<bool>>> getRotatedTiles() const;
    QString getFileName() const;
signals:
};

#endif // NETWORKPUZZLEFILE_H
