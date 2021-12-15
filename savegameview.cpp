#include "savegameview.h"
#include "ui_savegameview.h"

SaveGameDialog::SaveGameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveGameDialog)
{
    ui->setupUi(this);
    ui->gameFileTable->setHorizontalHeaderLabels(QStringList() << "File Name" << "Create Time");
    ui->gameFileTable->setRowCount(3);
}

void SaveGameDialog::resizeEvent(QResizeEvent *)
{
    ui->gameFileTable->setColumnWidth(0, this->width()*0.5);
}

SaveGameDialog::~SaveGameDialog()
{
    delete ui;
}
