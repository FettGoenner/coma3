#ifndef MAINWINDOWVIEW_H
#define MAINWINDOWVIEW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>

#include "gameview.h"
#include "newgamedialog.h"
#include "gamemodel.h"
#include "tileview.h"
#include "dockwindow.h"
#include "savegameview.h"

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
    SaveGameDialog *saveGame;
protected:
private slots:
    void on_newGameAction_triggered();
    void on_saveGameAction_triggered();
    void on_pauseAction_triggered();
    void on_resetAction_triggered();
    void on_exitAction_triggered();
    void on_loadGameAction_triggered();
    void on_actionAbout_2_triggered();
    void on_actionHelp_Page_triggered();
    void on_solutionAction_triggered();
    void on_hintAction_triggered();
};


#endif // MAINWINDOWVIEW_H
