#include "saveloadgameview.h"
#include "ui_saveloadgameview.h"



SaveLoadGameView::SaveLoadGameView(SaveLoadGameModel *model, SaveLoadGameView::SaveOrLoad status, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveGameDialog),
    m_model(model),
    m_status(status)
{
    ui->setupUi(this);


    connect(this, &SaveLoadGameView::deleteFile, m_model, &SaveLoadGameModel::deleteFile);
    connect(this, &SaveLoadGameView::saveBtnClicked, m_model, &SaveLoadGameModel::saveFile);
    connect(this, &SaveLoadGameView::loadBtnClicked, m_model, &SaveLoadGameModel::loadFile);

    if (status == SaveLoadGameView::Save){
        setWindowTitle("Save Game");
        ui->fileNameLineEdit->show();
        ui->fileNameLineEdit->setText(QDateTime::currentDateTime().toString("yy-MM-dd-hh-mm-ss"));
        loadCurrentData();
    }
    else {
        setWindowTitle("Load Game");
        ui->fileNameLineEdit->hide();
        ui->buttonBox->clear();

        ui->buttonBox->addButton("Cancel", QDialogButtonBox::RejectRole);
        ui->buttonBox->addButton("Load", QDialogButtonBox::AcceptRole);
        setSelectionsToDefault();
        clearLabels();
    }

    ui->gameFileTable->setColumnCount(2);
    ui->gameFileTable->setHorizontalHeaderLabels(QStringList() << "Filename" << "last modified time");

    // select by rows
    ui->gameFileTable->setSelectionBehavior(QTableWidget::SelectRows);

    // cannot select multiple rows
    ui->gameFileTable->setSelectionMode(QTableWidget::SingleSelection);

    // cannot edit the cells of the table
    ui->gameFileTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->deleteBtn->setEnabled(false);

    // set rows
    loadFiles();


    // set delete button enable if one file has been clicked
    connect(ui->gameFileTable, &QTableWidget::cellClicked, this, [=]() {
        ui->deleteBtn->setEnabled(true);
    });

    // delete button
    connect(ui->deleteBtn, &QPushButton::clicked, this, [=]() {
        if (ui->gameFileTable->currentRow() != -1) {
            QMessageBox msgBox(QMessageBox::Warning, "Delete?", "<b>Do you want to delete this File?</b>", QMessageBox::Yes | QMessageBox::Cancel, this);
            msgBox.setInformativeText("This step cannot be undone");
            msgBox.setDefaultButton(QMessageBox::Yes);
            if (msgBox.exec() == QMessageBox::Yes) {
                emit deleteFile(ui->gameFileTable->currentRow());
                loadFiles();
                setSelectionsToDefault();
                if (m_status == SaveLoadGameView::Load)
                    clearLabels();
            }
        }
    });

    // get informations from file
    connect(ui->gameFileTable, &QTableWidget::currentCellChanged, this, [=](int currentRow) {
        if (currentRow != -1) {
            ui->fileNameLineEdit->setText(ui->gameFileTable->item(currentRow, 0)->text());
            NetworkPuzzleFile *currentFile = m_model->getFiles()[currentRow];
            ui->sizeLabel->setText(QString::number(currentFile->getDim()));
            ui->seedLabel->setText(QString::number(currentFile->getSeed()));
            ui->algoLabel->setText(currentFile->getGameAlgo());
            ui->totalTimeLabel->setText(QTime(0, currentFile->getTotalPlayTime()/60, currentFile->getTotalPlayTime()%60).toString("mm:ss"));
            ui->totalStepsLabel->setText(QString::number(currentFile->getTotalSteps()));
        } else {
            setSelectionsToDefault();
        }
    });

    // set current row to -1 if the text from fileNameLineEdit changed.
    connect(ui->fileNameLineEdit, &QLineEdit::textEdited, this, [=]() {
        setSelectionsToDefault();
    });
}

void SaveLoadGameView::loadFiles()
{
    // load valid files from folder
    size_t gameFileSize = m_model->getFiles().size();
    ui->gameFileTable->setRowCount(gameFileSize);

    // set data
    for (size_t i = 0; i < gameFileSize; ++i) {
        ui->gameFileTable->setItem(i, 0, new QTableWidgetItem(m_model->getFiles()[i]->getFileName().left(m_model->getFiles()[i]->getFileName().size() - 5)));
        ui->gameFileTable->setItem(i, 1, new QTableWidgetItem(m_model->getFiles()[i]->lastModified().toString("yy-MM-dd-hh:mm:ss")));
    }
}

void SaveLoadGameView::loadCurrentData()
{
    ui->sizeLabel->setText(QString::number(m_model->currentGame->getSize()));
    ui->seedLabel->setText(QString::number(m_model->currentGame->getSeed()));
    ui->algoLabel->setText(m_model->currentGame->getAlgo());
    ui->totalTimeLabel->setText(QTime(0, m_model->currentGame->getTotalPlayTime()/60, m_model->currentGame->getTotalPlayTime()%60).toString("mm:ss"));
    ui->totalStepsLabel->setText(QString::number(m_model->currentGame->getTotalSteps()));
}

void SaveLoadGameView::setSelectionsToDefault()
{
    ui->gameFileTable->setCurrentItem(nullptr);
    loadCurrentData();
}

void SaveLoadGameView::accept()
{
    int currentRow = ui->gameFileTable->currentRow();
    if (m_status == SaveLoadGameView::Save) {
        bool save = true;
        QString fileName = ui->fileNameLineEdit->text();
        auto list = ui->gameFileTable->findItems(fileName, Qt::MatchFixedString);

        bool alreadyHere = list.size() == 0 ? false : true;
        if (alreadyHere)
            currentRow = list[0]->row();
        if (currentRow != -1 || alreadyHere) {
            QMessageBox msgBox(QMessageBox::Warning, "Overwrite?", "<b>Do you want to overwrite file "+ fileName +" ?</b>", QMessageBox::Yes | QMessageBox::Cancel, this);
            msgBox.setDefaultButton(QMessageBox::Yes);
            if (msgBox.exec() == QMessageBox::Cancel) {
                save = false;
            }
        }
        if (save) {
            emit saveBtnClicked(currentRow, fileName);
            setSelectionsToDefault();
            loadFiles();
        }
    } else {
        if (currentRow == -1) {
            QMessageBox msgBox(QMessageBox::Warning, "Choose a File", "<b>Please choose a file</b>", QMessageBox::Ok, this);
            msgBox.exec();
        } else {
            emit loadBtnClicked(currentRow);
            done(QDialog::Accepted);
        }
    }
}

void SaveLoadGameView::resizeEvent(QResizeEvent *)
{
    ui->gameFileTable->setColumnWidth(0, width()*0.5);
}

void SaveLoadGameView::clearLabels()
{
    ui->sizeLabel->setText("");
    ui->seedLabel->setText("");
    ui->algoLabel->setText("");
    ui->totalTimeLabel->setText("");
    ui->totalStepsLabel->setText("");
}

SaveLoadGameView::~SaveLoadGameView()
{
    clearLabels();
    delete ui;
}
