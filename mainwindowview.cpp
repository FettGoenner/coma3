#include "mainwindowview.h"
#include "ui_mainwindowview.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , seedStatusLabel(new QLabel)
    , algoStatusLabel(new QLabel)
    , sizeStatusLabel(new QLabel)
    , dockWindow(new DockWindow(this))
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

    this->addDockWidget(Qt::RightDockWidgetArea, this->dockWindow);
    this->gameModel = new GameModel(7, gameSeed);
    this->gameView = new GameView(this->gameModel, ui->gameWindow);
    connect(this->gameModel, &GameModel::sendGameSeed, this->seedStatusLabel, &QLabel::setText);
    // show the game window
    connect(this->gameModel, &GameModel::onGameInitialization, this, &MainWindow::showGameWindow);
    connect(this->gameModel, &GameModel::onGameStatus, this, &MainWindow::showGameWindow);
    connect(this->gameModel, &GameModel::gameStart, this, &MainWindow::showGameWindow);

    ui->gameWindow->layout()->addWidget(this->gameView);

    // pause buttom
    connect(this->dockWindow, &DockWindow::changedGameStarted, this->gameModel, &GameModel::changeGameStarted);
    connect(this->dockWindow, &DockWindow::changedGameStarted, this, [=](bool started) {
        if (started)
            this->showGameWindow();
        else
            this->hideGameWindow();
    });

    // solution button
    connect(this->dockWindow, &DockWindow::clickedSolutionBtn, this->gameModel, &GameModel::showSolution);


    // reset button
    connect(this->dockWindow, &DockWindow::clickedResetBtn, this->gameModel, &GameModel::resetGame);


    // new game button
    connect(this->dockWindow, &DockWindow::clickedNewGameBtn, this, [=]() {
        NewGameDialog dialog(this->gameModel->getSize(), this->gameModel->getAlgoType() , this);
        if (dialog.exec() == QDialog::Accepted) {

            // get alto type of new game
            GameModel::GameType algoType;
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


    // hint button
    connect(this->dockWindow, &DockWindow::clickedHintBtn, this->gameModel, &GameModel::showSolutionOnRandomTile);
    connect(this->gameModel, &GameModel::hintSuccessed, this->dockWindow, &DockWindow::setHintBtnText);

    // connect hint action and hint button
    connect(this->dockWindow, &DockWindow::hintBtnEnabledChanged, ui->hintAction, &QAction::setEnabled);

    // connect solution action and solution button
    connect(this->dockWindow, &DockWindow::solutionBtnEnabledChanged, ui->solutionAction, &QAction::setEnabled);

    // connect pause action and pause button
    connect(this->dockWindow, &DockWindow::pauseBtnEnabledChanged, ui->pauseAction, &QAction::setEnabled);
    connect(this->dockWindow, &DockWindow::pauseBtnTextChanged, ui->pauseAction, &QAction::setText);

//     count how many times HINT has been used.
//    connect(this->gameView, &GameView::hintSucceeded, this->dockWindow, &DockWindow::setHintCount);

    // send game started from GameModel
    connect(this->gameModel, &GameModel::gameStart, this->dockWindow, &DockWindow::gameStarted);

    // set text to totalSteps
    connect(this->gameModel, &GameModel::sendSteps, this->dockWindow, &DockWindow::setStep);

    // Timer
    connect(this->gameModel, &GameModel::sendTime, this->dockWindow, &DockWindow::setTime);

    // can not click hint button if the game has ended
    connect(this->gameModel, &GameModel::onGameStatus, this, [=](bool status) {
        if (status) // if there is a solution
            this->dockWindow->setHintBtnEnabled(!status); // set hint button to disable
    });

    // can not click solution button if the game has ended
    connect(this->gameModel, &GameModel::onGameStatus, this, [=](bool status) {
        if (status) // if there is a solution
            this->dockWindow->setSolutionBtnEnabled(!status); // set solution button to disable
    });
    // can not click solution button if the game has ended
    connect(this->gameModel, &GameModel::onGameStatus, this, [=](bool status) {
        if (status) // if there is a solution
            this->dockWindow->setPauseBtnEnabled(!status); // set pause button to disable
    });

}

void MainWindow::showGameWindow()
{
    ui->gameWindow->show();
}

void MainWindow::hideGameWindow()
{
    ui->gameWindow->hide();
}

// new game action
void MainWindow::on_newGameAction_triggered()
{
    this->dockWindow->clickNewGameBtn();
}

// save game action
void MainWindow::on_saveGameAction_triggered()
{
    if (this->saveLoadGameModel == nullptr)
        this->saveLoadGameModel = new SaveLoadGameModel(this->gameModel, this);

    if (this->saveLoadGameView == nullptr)
        this->saveLoadGameView = new SaveLoadGameView(saveLoadGameModel, SaveLoadGameView::Save, this);

    if (this->gameModel->gameStarted)
        this->dockWindow->clickPauseBtn();

    if (this->saveLoadGameControler == nullptr)
        this->saveLoadGameControler = new SaveLoadGameControler(this->saveLoadGameModel, this->saveLoadGameView, this);

    delete this->saveLoadGameModel;
    this->saveLoadGameModel = nullptr;

    delete this->saveLoadGameView;
    this->saveLoadGameView = nullptr;

    delete this->saveLoadGameControler;
    this->saveLoadGameControler = nullptr;

}

// load game action
void MainWindow::on_loadGameAction_triggered()
{
    if (this->saveLoadGameModel == nullptr)
        this->saveLoadGameModel = new SaveLoadGameModel(this->gameModel, this);

    if (this->saveLoadGameView == nullptr)
        this->saveLoadGameView = new SaveLoadGameView(saveLoadGameModel, SaveLoadGameView::Load, this);

    if (this->gameModel->gameStarted)
        this->dockWindow->clickPauseBtn();

    if (this->saveLoadGameControler == nullptr)
        this->saveLoadGameControler = new SaveLoadGameControler(this->saveLoadGameModel, this->saveLoadGameView, this);

    delete this->saveLoadGameModel;
    this->saveLoadGameModel = nullptr;

    delete this->saveLoadGameView;
    this->saveLoadGameView = nullptr;

    delete this->saveLoadGameControler;
    this->saveLoadGameControler = nullptr;
}

// pause action
void MainWindow::on_pauseAction_triggered()
{
    this->dockWindow->clickPauseBtn();
}

// reset action
void MainWindow::on_resetAction_triggered()
{
    this->dockWindow->clickResetBtn();
}



// exit action
void MainWindow::on_exitAction_triggered()
{
    this->close();
}

// Hint
void MainWindow::on_hintAction_triggered()
{
      this->dockWindow->clickHintBtn();
}

// Solution action

void MainWindow::on_solutionAction_triggered()
{
    this->dockWindow->clickSolutionBtn();
}

// About action
void MainWindow::on_aboutAction_triggered()
{
    QMessageBox::about(this, "About", "<h1>Netzwerkpuzzle</h1>"
                        "A Project at BHT, 2022<br><br>"
                        "<u><b>Autoren</b></u><br>"
                        "Xuantong Pan<br>"
                        "Paul Matti Meinhold<br>"
                        "Parfait R. Fejou<br>"
                        "Erdenetuya Undral<br>");
}

// Help action
void MainWindow::on_helpAction_triggered()
{
    QString link = "https://github.com/FettGoenner/coma3/tree/Milestone#readme";
    QDesktopServices::openUrl(QUrl(link));
}


MainWindow::~MainWindow()
{
    delete ui;
}



