#include "newgamedialog.h"
#include "ui_newgamedialog.h"
#include <QDebug>
#include <QRandomGenerator>

NewGameDialog::NewGameDialog(QWidget *parent, int row, int col) :
    QDialog(parent)
    , rows(row)
    , columns(col)
    , seed(this->getRandomValue())
    , ui(new Ui::NewGameDialog)
{
    ui->setupUi(this);
    ui->rowSpinBox->setValue(this->rows);
    ui->colSpinBox->setValue(this->columns);
    ui->seedSpinBox->setValue(this->seed);
    //get and set row value
    connect(ui->rowSpinBox, &QSpinBox::valueChanged, this, [=]() {
        this->rows = ui->rowSpinBox->value();
    });
    //get and set column value
    connect(ui->colSpinBox, &QSpinBox::valueChanged, this, [=]() {
        this->columns = ui->colSpinBox->value();
    });
    //get and set seed value
    connect(ui->seedSpinBox, &QSpinBox::valueChanged, this, [=]() {
        this->seed = ui->seedSpinBox->value();
    });

    connect(ui->getRandomSeedBtn, &QPushButton::clicked, this, [=]() {
        ui->seedSpinBox->setValue(this->getRandomValue());
    });

}

NewGameDialog::~NewGameDialog()
{
    delete ui;
}

int NewGameDialog::getRandomValue()
{
    return QRandomGenerator::global()->bounded(-999999999, 999999999);
}
