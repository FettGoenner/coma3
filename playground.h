#ifndef PLAYGROUND_H
#define PLAYGROUND_H

#include <QWidget>

class PlayGround : public QWidget
{
    Q_OBJECT
private:

public:
    explicit PlayGround(QWidget *parent = nullptr);
    bool gameStarted;
    int totalPlayTime;
    int totalSteps;
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
