#include "dockwindow.h"
#include "ui_dockwindow.h"

DockWindow::DockWindow(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::DockWindow)
{
    ui->setupUi(this);
    connect(ui->pauseBtn, &QPushButton::clicked, this, [=]() {
        if (ui->pauseBtn->text() == "Pause") {
            ui->pauseBtn->setText("Resume");
            emit this->changedGameStarted(false);
        } else {
            ui->pauseBtn->setText("Pause");
            emit this->changedGameStarted(true);
        }
    });
    connect(ui->solutionBtn, &QPushButton::clicked, this, [=]() {
        emit this->clickedSolutionBtn();
    });

    connect(ui->hintBtn, &QPushButton::clicked, this, [=]() {
        emit this->clickedHintBtn();
    });

    connect(ui->resetBtn, &QPushButton::clicked, this, [=]() {
        ui->totalPlayTime->setText("00:00");
        ui->totalSteps->setText("0");
        emit this->clickedResetBtn();
    });

    connect(ui->newGameBtn, &QPushButton::clicked, this, [=]() {
        emit this->clickedNewGameBtn();
    });
}

DockWindow::~DockWindow()
{
    delete ui;
}

void DockWindow::gameStarted()
{
    ui->pauseBtn->setText("Pause");
}

// set text to totalSteps
void DockWindow::setStep(QString steps)
{
    ui->totalSteps->setText(steps);
}

void DockWindow::setTime(QString totalTime)
{
    ui->totalPlayTime->setText(totalTime);
}

void DockWindow::newGameStarted()
{
    ui->totalPlayTime->setText("00:00");
    ui->totalSteps->setText("0");
}
