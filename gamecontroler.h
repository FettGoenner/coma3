#ifndef GAMECONTROLER_H
#define GAMECONTROLER_H

#include <QObject>

class QMouseEvent;
class QKeyEvent;

class GameModel;
class GameView;

class GameControler : public QObject
{
    Q_OBJECT
    //MVC: Die Seuerung kennt sowohl das Model, wie auch die View
    GameModel&  gameModel;
    GameView&   gameView;


public:
    explicit GameControler(GameModel& model, GameView& view, QObject * parent = nullptr);

    void mousePressEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent* event);

    // Eventfilter um die Ereignise von der Ansicht (QuadrateWidget) abzufangen
    bool eventFilter(QObject* watched, QEvent* event) override;
};

#endif // GAMECONTROLER_H
