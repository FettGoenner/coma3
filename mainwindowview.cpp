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
    ui->statusBar->addWidget(sizeStatusLabel);
    ui->statusBar->addWidget(seedStatusLabel);
    ui->statusBar->addWidget(algoStatusLabel);

    addDockWidget(Qt::RightDockWidgetArea, dockWindow);
    gameModel = new GameModel(7, gameSeed);
    gameView = new GameView(gameModel, ui->gameWindow);
    connect(gameModel, &GameModel::sendSeed, seedStatusLabel, &QLabel::setText);
    // show the game window
    connect(gameModel, &GameModel::onGameInit, this, &MainWindow::showGameWindow);
    connect(gameModel, &GameModel::onGameInit, this, [=]() {
        dockWindow->setSolutionBtnEnabled(true); // set solution button to disable
    });
    connect(gameModel, &GameModel::onGameStatus, this, &MainWindow::showGameWindow);
    connect(gameModel, &GameModel::gameStart, this, &MainWindow::showGameWindow);

    ui->gameWindow->layout()->addWidget(gameView);

    // pause buttom
    connect(dockWindow, &DockWindow::changedGameStarted, gameModel, &GameModel::changeGameStarted);
    connect(dockWindow, &DockWindow::changedGameStarted, this, [=](bool started) {
        if (started)
            showGameWindow();
        else
            hideGameWindow();
    });

    // solution button
    connect(dockWindow, &DockWindow::clickedSolutionBtn, gameModel, &GameModel::showSolution);


    // reset button
    connect(dockWindow, &DockWindow::clickedResetBtn, gameModel, &GameModel::resetGame);


    // new game button
    connect(dockWindow, &DockWindow::clickedNewGameBtn, this, [=]() {
        NewGameDialog dialog(gameModel->getSize(), gameModel->getAlgo() , this);
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
            gameModel->setSize(dialog.getSize());

            // get and set new game seed
            gameModel->setSeed(dialog.getSeed());

            // set algo type
            gameModel->initGame(algoType);

            // set everything in dock window to default.
            dockWindow->newGameStarted();

            // set game status to statusbar
            sizeStatusLabel->setText(QString("%1x%2").arg(dialog.getSize()).arg(dialog.getSize()));
            algoStatusLabel->setText(dialog.getAlgoType());
        }
    });


    // hint button
    connect(dockWindow, &DockWindow::clickedHintBtn, gameModel, &GameModel::showSolutionOnRandomTile);
    connect(gameModel, &GameModel::hintSuccessed, dockWindow, &DockWindow::setHintBtnText);

    // connect hint action and hint button
    connect(dockWindow, &DockWindow::hintBtnEnabledChanged, ui->hintAction, &QAction::setEnabled);

    // connect solution action and solution button
    connect(dockWindow, &DockWindow::solutionBtnEnabledChanged, ui->solutionAction, &QAction::setEnabled);

    // connect pause action and pause button
    connect(dockWindow, &DockWindow::pauseBtnEnabledChanged, ui->pauseAction, &QAction::setEnabled);
    connect(dockWindow, &DockWindow::pauseBtnTextChanged, ui->pauseAction, &QAction::setText);

    //     count how many times HINT has been used.
    //    connect(gameView, &GameView::hintSucceeded, dockWindow, &DockWindow::setHintCount);

    // send game started from GameModel
    connect(gameModel, &GameModel::gameStart, dockWindow, &DockWindow::gameStarted);

    // set text to totalSteps
    connect(gameModel, &GameModel::sendSteps, dockWindow, &DockWindow::setStep);

    // Timer
    connect(gameModel, &GameModel::sendTime, dockWindow, &DockWindow::setTime);

    // can not click hint button if the game has ended
    connect(gameModel, &GameModel::onGameStatus, this, [=](bool status) {
        if (status) // if there is a solution
            dockWindow->setHintBtnEnabled(!status); // set hint button to disable
    });

    // can not click solution button if the game has ended
    connect(gameModel, &GameModel::onGameStatus, this, [=](bool status) {
        if (status) // if there is a solution
            dockWindow->setSolutionBtnEnabled(!status); // set solution button to disable
    });
    // can not click solution button if the game has ended
    connect(gameModel, &GameModel::onGameStatus, this, [=](bool status) {
        if (status) // if there is a solution
            dockWindow->setPauseBtnEnabled(!status); // set pause button to disable
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
    dockWindow->clickNewGameBtn();
}

// save game action
void MainWindow::on_saveGameAction_triggered()
{
    if (saveLoadGameModel == nullptr)
        saveLoadGameModel = new SaveLoadGameModel(gameModel, this);

    if (saveLoadGameView == nullptr)
        saveLoadGameView = new SaveLoadGameView(saveLoadGameModel, SaveLoadGameView::Save, this);

    if (gameModel->started())
        dockWindow->clickPauseBtn();

    saveLoadGameView->exec();
    delete saveLoadGameModel;
    saveLoadGameModel = nullptr;

    delete saveLoadGameView;
    saveLoadGameView = nullptr;
}

// load game action
void MainWindow::on_loadGameAction_triggered()
{
    if (saveLoadGameModel == nullptr)
        saveLoadGameModel = new SaveLoadGameModel(gameModel, this);

    if (saveLoadGameView == nullptr)
        saveLoadGameView = new SaveLoadGameView(saveLoadGameModel, SaveLoadGameView::Load, this);

    if (gameModel->started())
        dockWindow->clickPauseBtn();
    saveLoadGameView->exec();

    delete saveLoadGameModel;
    saveLoadGameModel = nullptr;

    delete saveLoadGameView;
    saveLoadGameView = nullptr;
}
// pause action
void MainWindow::on_pauseAction_triggered()
{
    dockWindow->clickPauseBtn();
}

// reset action
void MainWindow::on_resetAction_triggered()
{
    dockWindow->clickResetBtn();
}



// exit action
void MainWindow::on_exitAction_triggered()
{
    close();
}

// Hint
void MainWindow::on_hintAction_triggered()
{
    dockWindow->clickHintBtn();
}

// Solution action

void MainWindow::on_solutionAction_triggered()
{
    dockWindow->clickSolutionBtn();
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



