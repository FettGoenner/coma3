#ifndef NETWORKPUZZLEFILE_H
#define NETWORKPUZZLEFILE_H

#include <QFile>
#include <QObject>

class NetworkPuzzleFile : public QFile
{
    Q_OBJECT
private:
    size_t dim;
    size_t seed;
    QString gameAlgo;
    size_t totalPlayTime;
    size_t totalSteps;

public:
    NetworkPuzzleFile();
    explicit NetworkPuzzleFile(QFile *parent = nullptr);
    explicit NetworkPuzzleFile(QString &fileName);


signals:

};

#endif // NETWORKPUZZLEFILE_H
