#include "dockwindow.h"
#include "ui_dockwindow.h"
#include <QString>

DockWindow::DockWindow(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::DockWindow)
{
    ui->setupUi(this);

    // pause button should be disabled by default
    this->setPauseBtnEnabled(false);
    // switch between "Pause" and "Resume"
    connect(ui->pauseBtn, &QPushButton::clicked, this, [=]() {
        if (ui->pauseBtn->text() == "Pause") {
            this->setPauseBtnText("Resume");
            emit this->changedGameStarted(false);

            // if the game stopped, solution button would enable
            this->setSolutionBtnEnabled(true);
        } else {
            ui->pauseBtn->setText("Pause");
            this->setPauseBtnText("Pause");
            emit this->changedGameStarted(true);

            // if the game resumed, solution button would disable
            this->setSolutionBtnEnabled(false);
        }
    });

    // send signal if clicked solution button
    connect(ui->solutionBtn, &QPushButton::clicked, this, [=]() {
        emit this->clickedSolutionBtn();
    });

    // send signal if clicked hint button
    connect(ui->hintBtn, &QPushButton::clicked, this, [=]() {
        emit this->clickedHintBtn();
    });

    // send signal if clicked reset button
    connect(ui->resetBtn, &QPushButton::clicked, this, [=]() {
        this->newGameStarted();

        emit this->clickedResetBtn();
    });

    // send signal if clicked new game button
    connect(ui->newGameBtn, &QPushButton::clicked, this, [=]() {
        emit this->clickedNewGameBtn();
    });
}

void DockWindow::setHintBtnText(int remaining)
{
    if (remaining == 0)
        this->setHintBtnEnabled(false);
    else
        this->setHintBtnEnabled(true);
    ui->hintBtn->setText(QString("Hint: %1").arg(remaining));
}

void DockWindow::setHintBtnEnabled(bool enabled)
{
    ui->hintBtn->setEnabled(enabled);
    emit this->hintBtnEnabledChanged(enabled);
}

void DockWindow::setSolutionBtnEnabled(bool enabled)
{
    ui->solutionBtn->setEnabled(enabled);
    emit this->solutionBtnEnabledChanged(enabled);
}

void DockWindow::setPauseBtnEnabled(bool enabled)
{
    ui->pauseBtn->setEnabled(enabled);
    emit pauseBtnEnabledChanged(enabled);
}

void DockWindow::setPauseBtnText(QString text)
{
    ui->pauseBtn->setText(text);
    emit this->pauseBtnTextChanged(text);
}

void DockWindow::gameStarted()
{
    this->setPauseBtnText("Pause");
    // after game started, solution button will be disabled
    this->setSolutionBtnEnabled(false);
    // after game started, pause button will be enabled
    this->setPauseBtnEnabled(true);
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
    this->setSolutionBtnEnabled(true);
    this->setPauseBtnEnabled(false);
}


void DockWindow::clickPauseBtn()
{
    ui->pauseBtn->click();
}

void DockWindow::clickHintBtn()
{
    ui->hintBtn->click();
}

void DockWindow::clickSolutionBtn()
{
    ui->solutionBtn->click();
}

void DockWindow::clickNewGameBtn()
{
    ui->newGameBtn->click();
}

void DockWindow::clickResetBtn()
{
    ui->resetBtn->click();
}


DockWindow::~DockWindow()
{
    delete ui;
}
