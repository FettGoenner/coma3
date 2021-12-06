#include "MainWindow.h"
#include "DockerWindow.h"
#include "GameModel.h"
#include "GameView.h"

#include <QDockWidget>
#include <QMenuBar>
#include <QAction>
#include <QLabel>

MainWindow::MainWindow(QWidget * parent)
    : QMainWindow(parent)
{
    // Zentrales Widget setzen
    int seed = QRandomGenerator::global()->bounded(-999999999, 999999999);
    GameModel * gameModel = new GameModel(7,7, seed) ;
    GameView * gameView = new GameView(gameModel, this) ;
    setCentralWidget(gameView) ;

    // Titel und minimale Breite/Höhe setzen
    setWindowTitle("Main Window") ;
    setMinimumWidth(400) ;
    setMinimumHeight(200) ;

    // Dock-Widget mit PushButton links anfügen
    QDockWidget * dockwidget = new QDockWidget ;
    dockwidget->setWidget(new DockerWindow(this)) ;
    addDockWidget(Qt::RightDockWidgetArea, dockwidget) ;

    createActions() ;
    createMenus() ;
}

void MainWindow::createActions()
{
    pauseAct = new QAction("Pause", this) ;
    resetAct = new QAction("Reset", this) ;
    newGameAct  = new QAction("New Game", this) ;
    saveGameAct = new QAction("Save Game", this) ;
    hintAct  = new QAction("Hint", this) ;
    aboutAct = new QAction("About", this) ;
    exitAct  = new QAction("Exit", this) ;
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu("File") ;
    fileMenu->addAction(pauseAct) ;
    fileMenu->addAction(resetAct) ;
    fileMenu->addAction(newGameAct) ;
    fileMenu->addAction(saveGameAct) ;
    fileMenu->addAction(hintAct) ;
    fileMenu->addAction(aboutAct) ;
    fileMenu->addAction(exitAct) ;
}

void MainWindow::pause()
{
    int x = 1 + 1 ;
}

void MainWindow::reset()
{
    int x = 1 + 1 ;
}

void MainWindow::newGame()
{
    int x = 1 + 1 ;
}

void MainWindow::saveGame()
{
    int x = 1 + 1 ;
}

void MainWindow::hint()
{
    int x = 1 + 1 ;
}

void MainWindow::about()
{
    int x = 1 + 1 ;
}

void MainWindow::exit()
{
    int x = 1 + 1 ;
}
