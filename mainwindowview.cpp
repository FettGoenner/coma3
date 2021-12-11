#include <QDebug>
#include <QTime>
#include <QHBoxLayout>

#include "mainwindowview.h"
#include "ui_mainwindowview.h"
#include "gameview.h"
#include "newgamedialog.h"
#include "gamemodel.h"
#include "tileview.h"

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

    GameModel *gameModel = new GameModel(7, gameSeed);
    GameView * gameView = new GameView(gameModel, ui->gameWindow);
    connect(gameModel, &GameModel::sendGameSeed, this->seedStatusLabel, &QLabel::setText);
    ui->gameWindow->layout()->addWidget(gameView);
    // Pause button
    connect(ui->pauseBtn, &QPushButton::clicked, this, [=]() {
        if (ui->pauseBtn->text() == "Pause") {
            ui->pauseBtn->setText("Resume");
            gameModel->gameStarted = false;
            //TODO
        } else {
            ui->pauseBtn->setText("Pause");
            gameModel->gameStarted = true;
            //TODO
        }
    });

    connect(gameModel, &GameModel::gameStart, ui->pauseBtn, [=]() {
        ui->pauseBtn->setText("Pause");
    });

    // set text to totalSteps
    connect(gameModel, &GameModel::sendSteps, ui->totalSteps_2, &QLabel::setText);
    // Timer
    connect(gameModel, &GameModel::sendTime, ui->totalPlayTime, &QLabel::setText);

    // new game
    connect(ui->newGameBtn, &QPushButton::clicked, this, [=]() {
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
            ui->totalPlayTime->setText("00:00");
            ui->totalSteps_2->setText("0");
            this->sizeStatusLabel->setText(QString("%1x%2").arg(dialog.getSize()).arg(dialog.getSize()));
            this->algoStatusLabel->setText(dialog.getAlgoType());
        }

    });

    // reset game
    connect(ui->resetBtn, &QPushButton::clicked, this, [=]() {
        gameModel->resetGame();
        ui->totalPlayTime->setText("00:00");
        ui->totalSteps_2->setText("0");
    });

    // solution
    connect(ui->solutionBtn, &QPushButton::clicked, gameModel, &GameModel::showSolution);

    // Hint
    connect(ui->hintBtn, &QPushButton::clicked, gameModel, &GameModel::startHint);
}

MainWindow::~MainWindow()
{
    delete ui;
}

