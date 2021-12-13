#include "dockwindow.h"
#include "ui_dockwindow.h"
#include <QString>

DockWindow::DockWindow(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::DockWindow)
{
    ui->setupUi(this);

    // pause button should be disabled by default
    ui->pauseBtn->setEnabled(false);

    // switch between "Pause" and "Resume"
    connect(ui->pauseBtn, &QPushButton::clicked, this, [=]() {
        if (ui->pauseBtn->text() == "Pause") {
            ui->pauseBtn->setText("Resume");

            emit this->changedGameStarted(false);

            // if the game stopped, solution button would enable
            ui->solutionBtn->setEnabled(true);
        } else {
            ui->pauseBtn->setText("Pause");

            emit this->changedGameStarted(true);

            // if the game resumed, solution button would disable
            ui->solutionBtn->setEnabled(false);
        }
    });
    connect(ui->solutionBtn, &QPushButton::clicked, this, [=]() {
        emit this->clickedSolutionBtn();
    });

    connect(ui->hintBtn, &QPushButton::clicked, this, [=]() {
        emit this->clickedHintBtn();
    });

    connect(ui->resetBtn, &QPushButton::clicked, this, [=]() {
        this->newGameStarted();

        emit this->clickedResetBtn();
    });

    connect(ui->newGameBtn, &QPushButton::clicked, this, [=]() {
        emit this->clickedNewGameBtn();
    });
}


void DockWindow::resetHint()
{
    ui->hintBtn->setEnabled(true);
    this->hintCount = 0;
    this->setHintBtnText();
}

void DockWindow::setHintBtnText()
{
    ui->hintBtn->setText(QString("Hint: %1").arg(DockWindow::HINTLIMIT - this->hintCount));
}

void DockWindow::gameStarted()
{
    ui->pauseBtn->setText("Pause");
    // after game started, solution button will be disabled
    ui->solutionBtn->setEnabled(false);

    // after game started, pause button will be enabled
    ui->pauseBtn->setEnabled(true);
}

// set text to totalSteps
void DockWindow::setStep(QString steps)
{
    ui->totalSteps->setText(steps);
}

// set time to totalPlayTime
void DockWindow::setTime(QString totalTime)
{
    ui->totalPlayTime->setText(totalTime);
}

// set everything to default
void DockWindow::newGameStarted()
{
    ui->pauseBtn->setText("Pause");
    ui->totalPlayTime->setText("00:00");
    ui->totalSteps->setText("0");
    ui->solutionBtn->setEnabled(true);
    ui->pauseBtn->setEnabled(false);

    //reset hint button
    this->resetHint();
}


void DockWindow::setHintCount()
{
    if (++this->hintCount >= DockWindow::HINTLIMIT)
        ui->hintBtn->setEnabled(false);

    this->setHintBtnText();
}


DockWindow::~DockWindow()
{
    delete ui;
}
