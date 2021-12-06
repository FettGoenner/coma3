#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QWidget>
#include <QVector>
#include <QGridLayout>
#include <QRandomGenerator>
#include <QTimer>
#include <QTime>

#include "tilemodel.h"
#include "gamemodel.h"
#include "tileview.h"

class GameView : public QWidget
{
    Q_OBJECT
private:
    GameModel *gameModel;
    QGridLayout *gridLayout = nullptr;
    QColor bgc = Qt::white;

    void clearLayout();
public:
    explicit GameView(GameModel *gameModel, QWidget *parent = nullptr);

public slots:
    void showGame(bool clearStatus = false);
    void changeBgc(bool connected);
protected:
    void paintEvent(QPaintEvent*) override;
};

#endif // GAMEVIEW_H
