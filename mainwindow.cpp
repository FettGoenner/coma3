#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "straightnode.h"
#include "turnnode.h"
#include "playground.h"
#include <QDebug>
#include <QTime>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , timer(new QTimer(this))

{
    ui->setupUi(this);

    // Let playground has same size as ui->gameWindow
    QHBoxLayout *layout = new QHBoxLayout(this);
    ui->gameWindow->setLayout(layout);

    PlayGround * playground = new PlayGround(ui->gameWindow, 3);
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
    connect(timer, &QTimer::timeout, this, [=]() {
        if (playground->gameStarted) {
            ++playground->totalPlayTime;
            int minute = playground->totalPlayTime/60;
            int sec = playground->totalPlayTime%60;
            QTime time(0, minute, sec);
            ui->totalPlayTime->setText(time.toString("mm:ss"));
        }
    });
    timer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

