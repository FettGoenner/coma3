#ifndef PLAYGROUND_H
#define PLAYGROUND_H

#include <QWidget>
#include <QVector>
#include "node.h"

class PlayGround : public QWidget
{
    Q_OBJECT
private:

public:
    explicit PlayGround(QWidget *parent = nullptr, int playGroundSize = 7);
    bool gameStarted;
    int totalPlayTime;
    int totalSteps;
    int playGroundSize;
    QVector<QVector<Node*>> playGround;
protected:
    void paintEvent(QPaintEvent*) override;
signals:
    QString sendSteps(QString totalsteps);
public slots:
    void setSteps() {
        if (!gameStarted)
            gameStarted = true;
        ++totalSteps;
        emit sendSteps(QString::number(totalSteps));
    }
};

#endif // PLAYGROUND_H
