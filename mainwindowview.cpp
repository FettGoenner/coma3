#include <QTime>
#include <QHBoxLayout>
#include <QKeyEvent>

#include "mainwindowview.h"
#include "ui_mainwindowview.h"

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

    this->dockWindow = new DockWindow(this);
    this->addDockWidget(Qt::RightDockWidgetArea, this->dockWindow);
    this->gameModel = new GameModel(7, gameSeed);
    this->gameView = new GameView(this->gameModel, ui->gameWindow);
    connect(this->gameModel, &GameModel::sendGameSeed, this->seedStatusLabel, &QLabel::setText);
    ui->gameWindow->layout()->addWidget(this->gameView);

    // pause buttom
    connect(this->dockWindow, &DockWindow::changedGameStarted, this->gameModel, &GameModel::changeGameStarted);

    // solution button
    connect(this->dockWindow, &DockWindow::clickedSolutionBtn, this->gameModel, &GameModel::showSolution);

    // hint button
    connect(this->dockWindow, &DockWindow::clickedHintBtn, this->gameView, &GameView::startHint);

    // reset button
    connect(this->dockWindow, &DockWindow::clickedResetBtn, this->gameModel, &GameModel::resetGame);

    // new game button
    connect(this->dockWindow, &DockWindow::clickedNewGameBtn, this, [=]() {
        NewGameDialog dialog(this->gameModel->getSize(), this->gameModel->getAlgoType() , this);
        if (dialog.exec() == QDialog::Accepted) {

            // get alto type of new game
            int algoType = 0;
            if (dialog.getAlgoType() == "Depth")
                algoType = GameModel::Depth;
            else if (dialog.getAlgoType() == "Prim")
                algoType = GameModel::Prim;
            else
                throw "unknown game type";

            // get and set new game size
            this->gameModel->setSize(dialog.getSize());

            // get and set new game seed
            this->gameModel->setGameSeed(dialog.getSeed());

            // set algo type
            this->gameModel->initializeGame(algoType);

            // set everything in dock window to default.
            this->dockWindow->newGameStarted();

            // set game status to statusbar
            this->sizeStatusLabel->setText(QString("%1x%2").arg(dialog.getSize()).arg(dialog.getSize()));
            this->algoStatusLabel->setText(dialog.getAlgoType());
        }
    });

    // connect hint action and hint button
    connect(this->dockWindow, &DockWindow::hintBtnEnabledChanged, ui->hintAction, &QAction::setEnabled);

    // connect solution action and solution button
    connect(this->dockWindow, &DockWindow::solutionBtnEnabledChanged, ui->solutionAction, &QAction::setEnabled);

    // connect pause action and pause button
    connect(this->dockWindow, &DockWindow::pauseBtnEnabledChanged, ui->pauseAction, &QAction::setEnabled);
    connect(this->dockWindow, &DockWindow::pauseBtnTextChanged, ui->pauseAction, &QAction::setText);

    // new game action
    connect(ui->newGameAction, &QAction::triggered, this->dockWindow, &DockWindow::clickNewGameBtn);

    // reset action
    connect(ui->resetAction, &QAction::triggered, this->dockWindow, &DockWindow::clickResetBtn);

    //hint action
    connect(ui->hintAction, &QAction::triggered, this->dockWindow, &DockWindow::clickHintBtn);

    // pause action
    connect(ui->pauseAction, &QAction::triggered, this->dockWindow, &DockWindow::clickPauseBtn);

    // solution action
    connect(ui->solutionAction, &QAction::triggered, this->dockWindow, &DockWindow::clickSolutionBtn);

    // save game action
    // TODO

    // about action
    // TODO

    // exit action
    connect(ui->exitAction, &QAction::triggered, this, [=]() {
        this->close();
    });

    // count how many times HINT has been used.
    connect(gameView, &GameView::hintSucceeded, this->dockWindow, &DockWindow::setHintCount);

    // send game started from GameModel
    connect(gameModel, &GameModel::gameStart, this->dockWindow, &DockWindow::gameStarted);

    // set text to totalSteps
    connect(gameModel, &GameModel::sendSteps, this->dockWindow, &DockWindow::setStep);
    // Timer
    connect(gameModel, &GameModel::sendTime, this->dockWindow, &DockWindow::setTime);

}


void MainWindow::keyPressEvent(QKeyEvent *ev)
{
    if (ev->modifiers() == Qt::ControlModifier && ev->key() == Qt::Key_N)
        this->dockWindow->clickNewGameBtn();
    else if (ev->modifiers() == Qt::ControlModifier && ev->key() == Qt::Key_R)
        this->dockWindow->clickResetBtn();
    else if (ev->modifiers() == Qt::ControlModifier && ev->key() == Qt::Key_H)
        this->dockWindow->clickHintBtn();
    else if (ev->modifiers() == Qt::AltModifier && ev->key() == Qt::Key_S)
        this->dockWindow->clickSolutionBtn();
//    else if (ev->modifiers() == Qt::ControlModifier && ev->key() == Qt::Key_S)
//        this->dockWindow->clickHintBtn();
    else if (ev->modifiers() == Qt::ControlModifier && ev->key() == Qt::Key_W)
        this->close();
    else if (ev->key() == Qt::Key_F9)
        this->dockWindow->clickPauseBtn();
}


MainWindow::~MainWindow()
{
    delete ui;
}

