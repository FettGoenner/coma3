#include "savegameview.h"
#include "ui_savegameview.h"

SaveGameDialog::SaveGameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveGameDialog)
{
    ui->setupUi(this);
    ui->gameFileTable->setColumnCount(2);
    ui->gameFileTable->setHorizontalHeaderLabels(QStringList() << "File Name" << "Update Time");
    ui->gameFileTable->setRowCount(3);
    ui->gameFileTable->setItem(0, 0, new QTableWidgetItem("111"));
    ui->gameFileTable->setItem(1, 0, new QTableWidgetItem("222"));
    ui->gameFileTable->setItem(2, 0, new QTableWidgetItem("333"));
    // select by rows
    ui->gameFileTable->setSelectionBehavior(QTableWidget::SelectRows);

    // cannot select multiple rows
    ui->gameFileTable->setSelectionMode(QTableWidget::SingleSelection);

    // cannot edit the cells of the table
    ui->gameFileTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->gameFileTable->setHorizontalHeaderLabels(QStringList() << "File Name" << "Create Time");
    ui->deleteBtn->setEnabled(false);
    connect(ui->gameFileTable, &QTableWidget::cellClicked, this, [=]() {
        ui->deleteBtn->setEnabled(true);
    });
    connect(ui->addBtn, &QPushButton::clicked, this, [=]() {
        if (ui->gameFileTable->currentRow() == -1)
            ui->gameFileTable->insertRow(ui->gameFileTable->rowCount());
        else
            ui->gameFileTable->insertRow(ui->gameFileTable->currentRow() + 1);
    });

    connect(ui->deleteBtn, &QPushButton::clicked, this, [=]() {
        ui->gameFileTable->removeRow(ui->gameFileTable->currentRow());
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
