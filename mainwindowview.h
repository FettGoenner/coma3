#ifndef MAINWINDOWVIEW_H
#define MAINWINDOWVIEW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QTime>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QMessageBox>
#include <QDesktopServices>
#include <QTextBrowser>

#include "gameview.h"
#include "newgamedialog.h"
#include "gamemodel.h"
#include "tileview.h"
#include "dockwindow.h"

#include "saveloadgamemodel.h"
#include "saveloadgameview.h"
#include "saveloadgamecontroler.h"

#include "networkpuzzlefile.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QLabel *seedStatusLabel;
    QLabel *algoStatusLabel;
    QLabel *sizeStatusLabel;

    DockWindow *dockWindow;
    GameModel *gameModel;
    GameView *gameView;

    SaveLoadGameModel *saveLoadGameModel = nullptr;
    SaveLoadGameView *saveLoadGameView = nullptr;
    SaveLoadGameControler *saveLoadGameControler = nullptr;

    void showGameWindow();
    void hideGameWindow();
protected:
private slots:
    void on_newGameAction_triggered();
    void on_saveGameAction_triggered();
    void on_pauseAction_triggered();
    void on_resetAction_triggered();
    void on_hintAction_triggered();
    void on_exitAction_triggered();
    void on_solutionAction_triggered();
    void on_loadGameAction_triggered();
    void on_aboutAction_triggered();
    void on_helpAction_triggered();
};


#endif // MAINWINDOWVIEW_H
