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
    QFile _gameFile;
    QString _fileName;
    QJsonObject _gameObj;

    size_t _dim;
    size_t _seed;
    QString _gameAlgo;
    size_t _totalPlayTime;
    size_t _totalSteps;
    size_t _hintRemaining;
    bool _dataValid;
    QVector<QPair<size_t, size_t>> _hintedTiles; // QPair<size_t, size_t> saves the position of the tiles
    QVector<QPair<QPair<size_t, size_t>, QVector<bool>>> _rotatedTiles; // QVector<int> saves the nodes of the tiles
public:
    static const QString GAME_FILE_PATH;

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
