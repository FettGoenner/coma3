#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "straightnode.h"
#include "turnnode.h"
#include <QDebug>
#include <QTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , totalSteps(0)
    , timer(new QTimer(this))
    , gameStarted(false)
    , totalPlayTime(0)
{
    ui->setupUi(this);

    connect(ui->pauseBtn, &QPushButton::clicked, this, [=]() {
        if (ui->pauseBtn->text() == "Pause") {
            ui->pauseBtn->setText("Resume");
            gameStarted = false;
            //TODO
        } else {
            ui->pauseBtn->setText("Pause");
            gameStarted = true;
            //TODO
        }
    });
    // test
    StraightNode * sNode = new StraightNode(Qt::blue);
    sNode->setParent(ui->gameWindow);
    sNode->resize(200, 200);
    sNode->move(0, 200);
    TurnNode * tNode = new TurnNode(Qt::blue);
    tNode->resize(200, 200);
    tNode->setParent(ui->gameWindow);
    connect(tNode, &Node::clicked, this, &MainWindow::setSteps);
    connect(sNode, &Node::clicked, this, &MainWindow::setSteps);

    // set text to totalSteps
    connect(this, &MainWindow::sendSteps, ui->totalSteps_2, &QLabel::setText);
    // Timer
    connect(timer, &QTimer::timeout, this, [=]() {
        if (gameStarted) {
            ++totalPlayTime;
            int minute = totalPlayTime/60;
            int sec = totalPlayTime%60;
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

