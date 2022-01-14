#include <QTime>
#include <QDate>
#include "savegameview.h"
#include "ui_savegameview.h"
#include "networkpuzzlefile.h"

SaveGameDialog::SaveGameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveGameDialog)
{
    ui->setupUi(this);
    ui->gameFileTable->setColumnCount(2);
    ui->gameFileTable->setHorizontalHeaderLabels(QStringList() << "Filename" << "last modified time");

    // select by rows
    ui->gameFileTable->setSelectionBehavior(QTableWidget::SelectRows);

    // cannot select multiple rows
    ui->gameFileTable->setSelectionMode(QTableWidget::SingleSelection);

    // cannot edit the cells of the table
    ui->gameFileTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->deleteBtn->setEnabled(false);

    SaveGameModel *saveGameModel = new SaveGameModel;
    QVector<QPair<NetworkPuzzleFile*, QDateTime>> gameFiles = saveGameModel->getFiles();

    size_t gameFileSize = gameFiles.size();
    ui->gameFileTable->setRowCount(gameFileSize);
    for (size_t i = 0; i < gameFileSize; ++i) {
        ui->gameFileTable->setItem(i, 0, new QTableWidgetItem(gameFiles[i].first->getFileName().left(gameFiles[i].first->getFileName().size() - 5)));
        ui->gameFileTable->setItem(i, 1, new QTableWidgetItem(gameFiles[i].second.toString("yy-MM-dd-hh:mm:ss")));
    }

    connect(ui->gameFileTable, &QTableWidget::cellClicked, this, [=]() {
        ui->deleteBtn->setEnabled(true);
    });
    connect(ui->addBtn, &QPushButton::clicked, this, [=]() {
        if (ui->gameFileTable->currentRow() == -1) {
            ui->gameFileTable->insertRow(ui->gameFileTable->rowCount());
            ui->gameFileTable->setItem(ui->gameFileTable->rowCount() - 1, 0, new QTableWidgetItem(QDate::currentDate().toString("yy-MM-dd_")+QTime::currentTime().toString("hh:mm:ss")));
            ui->gameFileTable->setCurrentCell(ui->gameFileTable->rowCount() - 1, 0);
        }
        else {
            ui->gameFileTable->insertRow(ui->gameFileTable->currentRow() + 1);
            ui->gameFileTable->setItem(ui->gameFileTable->currentRow() + 1, 0, new QTableWidgetItem(QDate::currentDate().toString("yy-MM-dd_")+QTime::currentTime().toString("hh:mm:ss")));
            ui->gameFileTable->setCurrentCell(ui->gameFileTable->currentRow() + 1, 0);
        }
    });

    connect(ui->deleteBtn, &QPushButton::clicked, this, [=]() {
        ui->gameFileTable->removeRow(ui->gameFileTable->currentRow());
    });
    connect(ui->gameFileTable, &QTableWidget::currentCellChanged, this, [=](int currentRow) {
        ui->fileNameLineEdit->setText(ui->gameFileTable->item(currentRow, 0)->text());
        NetworkPuzzleFile *currentFile = gameFiles[currentRow].first;
        ui->sizeLabel->setText(QString::number(currentFile->getDim()));
        ui->seedLabel->setText(QString::number(currentFile->getSeed()));
        ui->algoLabel->setText(currentFile->getGameAlgo());
        ui->totalTimeLabel->setText(QTime(0, currentFile->getTotalPlayTime()/60, currentFile->getTotalPlayTime()%60).toString("mm:ss"));
        ui->totalStepsLabel->setText(QString::number(currentFile->getTotalSteps()));
    });

}

void SaveGameDialog::resizeEvent(QResizeEvent *)
{
    ui->gameFileTable->setColumnWidth(0, this->width()*0.5);
}

SaveGameDialog::~SaveGameDialog()
{
    delete ui;
}
