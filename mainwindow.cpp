#include <QDebug>
#include <QTime>
#include <QHBoxLayout>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "linetile.h"
#include "cornertile.h"
#include "playground.h"
#include "newgamedialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , statusLabel(new QLabel)
{
    ui->setupUi(this);

    // Let playground has same size as ui->gameWindow
    QHBoxLayout *layout = new QHBoxLayout;
    ui->gameWindow->setLayout(layout);
    //set statusbar
    int gameSeed = QRandomGenerator::global()->bounded(-999999999, 999999999);
    statusLabel->setText(QString::number(gameSeed));
    ui->statusBar->addWidget(this->statusLabel);

    PlayGround * playground = new PlayGround(ui->gameWindow, 7, 7, gameSeed);
    connect(playground, &PlayGround::sendGameSeed, this->statusLabel, &QLabel::setText);
    ui->gameWindow->layout()->addWidget(playground);
    // Pause button
    connect(ui->pauseBtn, &QPushButton::clicked, this, [=]() {
        if (ui->pauseBtn->text() == "Pause") {
            ui->pauseBtn->setText("Resume");
            playground->gameStarted = false;
            //TODO
        } else {
            ui->pauseBtn->setText("Pause");
            playground->gameStarted = true;
            //TODO
        }
    });

    // set text to totalSteps
    connect(playground, &PlayGround::sendSteps, ui->totalSteps_2, &QLabel::setText);
    // Timer
    connect(playground, &PlayGround::sendTime, ui->totalPlayTime, &QLabel::setText);

    connect(ui->newGameBtn, &QPushButton::clicked, this, [=]() {
        NewGameDialog dialog(this, playground->playGroundSizeY, playground->playGroundSizeX);
        if (dialog.exec() == QDialog::Accepted) {
            playground->setSize(dialog.rows, dialog.columns);
            playground->setGameSeed(dialog.seed);
            playground->initializeGame();
            ui->totalPlayTime->setText("00:00");
            ui->totalSteps_2->setText("0");
        }

    });
    connect(ui->resetBtn, &QPushButton::clicked, this, [=]() {
        playground->resetGame();
        ui->totalPlayTime->setText("00:00");
        ui->totalSteps_2->setText("0");
    });
    connect(ui->solutionBtn, &QPushButton::clicked, playground, &PlayGround::showSolution);
}

MainWindow::~MainWindow()
{
    delete ui;
}

