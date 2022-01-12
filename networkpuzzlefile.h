#ifndef NETWORKPUZZLEFILE_H
#define NETWORKPUZZLEFILE_H

#include <QFile>
#include <QObject>
#include <QVector>
#include <QPair>

class NetworkPuzzleFile : public QObject
{
    Q_OBJECT
private:
    QFile gameFile;
    size_t dim;
    size_t seed;
    QString gameAlgo;
    size_t totalPlayTime;
    size_t totalSteps;
    size_t hintRemaning;
    QVector<QPair<size_t, size_t>> hintedTile;
//    QVector<QVector<QVector<bool>>> game;

public:
    NetworkPuzzleFile();
    explicit NetworkPuzzleFile(QObject *parent = nullptr);
    NetworkPuzzleFile(const QString &fileName, QObject *parent = nullptr);
    void loadData();
signals:
    void failedToReadFile();
};

#endif // NETWORKPUZZLEFILE_H
