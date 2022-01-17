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
#include <QDateTime>
#include <QFileInfo>
#include <QDir>

#include "gamemodel.h"

class NetworkPuzzleFile : public QObject
{
    Q_OBJECT
private:
    QFile gameFile;
    QString fileName;
    QJsonObject gameObj;

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
    static const QString GAME_FILE_PATH;

    NetworkPuzzleFile();
    explicit NetworkPuzzleFile(QObject *parent = nullptr);
    NetworkPuzzleFile(const QString &fileName = "", QObject *parent = nullptr);
    void loadData();
    bool writeData();
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
    QDateTime lastModified() const;

    void updateData(GameModel &gameModel, QString fileName = "");
    void deleteFile();
signals:
};

#endif // NETWORKPUZZLEFILE_H
