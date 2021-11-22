#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "straightnode.h"
#include "turnnode.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , totalSteps(0)
{
    ui->setupUi(this);
    connect(ui->pauseBtn, &QPushButton::clicked, this, [=]() {
        if (ui->pauseBtn->text() == "Pause") {
            ui->pauseBtn->setText("Resume");
        } else {
            ui->pauseBtn->setText("Pause");
        }
    });
    StraightNode * sNode = new StraightNode(Qt::red);
    sNode->setParent(ui->gameWindow);
    sNode->resize(200, 200);
    sNode->move(0, 200);
    TurnNode * tNode = new TurnNode(Qt::red);
    tNode->resize(200, 200);
    tNode->setParent(ui->gameWindow);
    connect(tNode, &Node::clicked, this, &MainWindow::setSteps);
    connect(this, &MainWindow::sendSteps, ui->totalSteps_2, &QLabel::setText);
}

MainWindow::~MainWindow()
{
    delete ui;
}

