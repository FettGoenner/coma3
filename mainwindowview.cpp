#include <QDebug>
#include <QTime>
#include <QHBoxLayout>

#include "mainwindowview.h"
#include "ui_mainwindowview.h"
#include "gameview.h"
#include "newgamedialog.h"
#include "gamemodel.h"
#include "tileview.h"
#include "dockwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , seedStatusLabel(new QLabel)
    , algoStatusLabel(new QLabel)
    , sizeStatusLabel(new QLabel)
{
    ui->setupUi(this);

    // Let playground has same size as ui->gameWindow
    QHBoxLayout *layout = new QHBoxLayout;
    ui->gameWindow->setLayout(layout);
    //set statusbar
    size_t gameSeed = QRandomGenerator::global()->bounded(0, INT_MAX);

    seedStatusLabel->setText(QString::number(gameSeed));
    algoStatusLabel->setText("Depth");
    sizeStatusLabel->setText("7x7");
    ui->statusBar->addWidget(this->sizeStatusLabel);
    ui->statusBar->addWidget(this->seedStatusLabel);
    ui->statusBar->addWidget(this->algoStatusLabel);

    DockWindow *dockWindow = new DockWindow(this);
    this->addDockWidget(Qt::RightDockWidgetArea, dockWindow);
    GameModel *gameModel = new GameModel(7, gameSeed);
    GameView * gameView = new GameView(gameModel, ui->gameWindow);
    connect(gameModel, &GameModel::sendGameSeed, this->seedStatusLabel, &QLabel::setText);
    ui->gameWindow->layout()->addWidget(gameView);

    connect(dockWindow, &DockWindow::changedGameStarted, gameModel, &GameModel::changeGameStarted);

    connect(dockWindow, &DockWindow::clickedSolutionBtn, gameModel, &GameModel::showSolution);

    connect(dockWindow, &DockWindow::clickedHintBtn, gameView, &GameView::startHint);

    connect(dockWindow, &DockWindow::clickedResetBtn, gameModel, &GameModel::resetGame);

    connect(gameModel, &GameModel::gameStart, dockWindow, &DockWindow::gameStarted);

    // set text to totalSteps
    connect(gameModel, &GameModel::sendSteps, dockWindow, &DockWindow::setStep);
    // Timer
    connect(gameModel, &GameModel::sendTime, dockWindow, &DockWindow::setTime);
//    // new game
    connect(dockWindow, &DockWindow::clickedNewGameBtn, this, [=]() {
        NewGameDialog dialog(gameModel->getSize(), gameModel->getAlgoType() , this);
        if (dialog.exec() == QDialog::Accepted) {
            int algoType = 0;
            if (dialog.getAlgoType() == "Depth")
                algoType = GameModel::Depth;
            else if (dialog.getAlgoType() == "Prim")
                algoType = GameModel::Prim;
            else

                throw "unknown game type";
            gameModel->setSize(dialog.getSize());
            gameModel->setGameSeed(dialog.getSeed());
            gameModel->initializeGame(algoType);
            dockWindow->newGameStarted();
            this->sizeStatusLabel->setText(QString("%1x%2").arg(dialog.getSize()).arg(dialog.getSize()));
            this->algoStatusLabel->setText(dialog.getAlgoType());
        }

    });


}

MainWindow::~MainWindow()
{
    delete ui;
}

